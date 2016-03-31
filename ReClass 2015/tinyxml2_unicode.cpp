#include "stdafx.h"
/*
Original code by Lee Thomason (www.grinninglizard.com)
wchar_t port by dUkk


This software is provided 'as-is', without any express or implied
warranty. In no event will the authors be held liable for any
damages arising from the use of this software.

Permission is granted to anyone to use this software for any
purpose, including commercial applications, and to alter it and
redistribute it freely, subject to the following restrictions:

1. The origin of this software must not be misrepresented; you must
not claim that you wrote the original software. If you use this
software in a product, an acknowledgment in the product documentation
would be appreciated but is not required.

2. Altered source versions must be plainly marked as such, and
must not be misrepresented as being the original software.

3. This notice may not be removed or altered from any source
distribution.
*/

#include "tinyxml2_unicode.h"

#include <new>		// yes, this one new style header, is in the Android SDK.
#   ifdef ANDROID_NDK
#   include <stddef.h>
#else
#   include <cstddef>
#endif

static const char LINE_FEED				= (char)0x0a;			// all line endings are normalized to LF
static const char LF = LINE_FEED;
static const char CARRIAGE_RETURN		= (char)0x0d;			// CR gets filtered out
static const char CR = CARRIAGE_RETURN;
static const char SINGLE_QUOTE			= '\'';
static const char DOUBLE_QUOTE			= '\"';

// Bunch of unicode info at:
//		http://www.unicode.org/faq/utf_bom.html
//	ef bb bf (Microsoft "lead bytes") - designates UTF-8

static const unsigned char TIXML_UTF_LEAD_0 = 0xefU;
static const unsigned char TIXML_UTF_LEAD_1 = 0xbbU;
static const unsigned char TIXML_UTF_LEAD_2 = 0xbfU;


#define DELETE_NODE( node )	{			\
        if ( node ) {						\
            MemPool* pool = node->_memPool;	\
            node->~XMLNode();				\
            pool->Free( node );				\
        }									\
    }
#define DELETE_ATTRIBUTE( attrib ) {		\
        if ( attrib ) {							\
            MemPool* pool = attrib->_memPool;	\
            attrib->~XMLAttribute();			\
            pool->Free( attrib );				\
        }										\
    }

namespace tinyxml2
{

struct Entity {
    const wchar_t* pattern;
    int length;
    wchar_t value;
};

static const int NUM_ENTITIES = 5;
static const Entity entities[NUM_ENTITIES] = {
    { L"quot", 4,	DOUBLE_QUOTE },
    { L"amp", 3,		'&'  },
    { L"apos", 4,	SINGLE_QUOTE },
    { L"lt",	2, 		'<'	 },
    { L"gt",	2,		'>'	 }
};


StrPair::~StrPair()
{
    Reset();
}


void StrPair::Reset()
{
    if ( _flags & NEEDS_DELETE ) {
        delete [] _start;
    }
    _flags = 0;
    _start = 0;
    _end = 0;
}


void StrPair::SetStr( const wchar_t* str, int flags )
{
    Reset();
    size_t len = wcslen( str );
    _start = new wchar_t[ len+1 ];
    wmemcpy( _start, str, len+1 );
    _end = _start + len;
    _flags = flags | NEEDS_DELETE;
}


wchar_t* StrPair::ParseText( wchar_t* p, const wchar_t* endTag, int strFlags )
{
    TIXMLASSERT( endTag && *endTag );

    wchar_t* start = p;	// fixme: hides a member
    wchar_t  endChar = *endTag;
    size_t length = wcslen( endTag );

    // Inner loop of text parsing.
    while ( *p ) {
        if ( *p == endChar && wcsncmp( p, endTag, length ) == 0 ) {
            Set( start, p, strFlags );
            return p + length;
        }
        ++p;
    }
    return 0;
}


wchar_t* StrPair::ParseName( wchar_t* p )
{
    wchar_t* start = p;

    if ( !start || !(*start) ) {
        return 0;
    }

    while( *p && (
                XMLUtil::IsAlphaNum( (unsigned char) *p )
                || *p == '_'
                || *p == ':'
                || (*p == '-' && p>start )		// can be in a name, but not lead it.
                || (*p == '.' && p>start ) )) {	// can be in a name, but not lead it.
        ++p;
    }

    if ( p > start ) {
        Set( start, p, 0 );
        return p;
    }
    return 0;
}


void StrPair::CollapseWhitespace()
{
    // Trim leading space.
    _start = XMLUtil::SkipWhiteSpace( _start );

    if ( _start && *_start ) {
        wchar_t* p = _start;	// the read pointer
        wchar_t* q = _start;	// the write pointer

        while( *p ) {
            if ( XMLUtil::IsWhiteSpace( *p )) {
                p = XMLUtil::SkipWhiteSpace( p );
                if ( *p == 0 ) {
                    break;    // don't write to q; this trims the trailing space.
                }
                *q = ' ';
                ++q;
            }
            *q = *p;
            ++q;
            ++p;
        }
        *q = 0;
    }
}

const wchar_t* StrPair::GetStr()
{
    if ( _flags & NEEDS_FLUSH ) {
        *_end = 0;
        _flags ^= NEEDS_FLUSH;

        if ( _flags ) {
            wchar_t* p = _start;	// the read pointer
			wchar_t* q = _start;	// the write pointer

            while( p < _end ) {
                if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == CR ) {
                    // CR-LF pair becomes LF
                    // CR alone becomes LF
                    // LF-CR becomes LF
                    if ( *(p+1) == LF ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_NEWLINE_NORMALIZATION) && *p == LF ) {
                    if ( *(p+1) == CR ) {
                        p += 2;
                    }
                    else {
                        ++p;
                    }
                    *q++ = LF;
                }
                else if ( (_flags & NEEDS_ENTITY_PROCESSING) && *p == '&' ) {
                    // Entities handled by tinyXML2:
                    // - special entities in the entity table [in/out]
                    // - numeric character reference [in]
                    //   &#20013; or &#x4e2d;

                    if ( *(p+1) == '#' ) {
                        wchar_t buf[10] = { 0 };
                        int len;
                        p = const_cast<wchar_t*>( XMLUtil::GetCharacterRef( p, buf, &len ) );
                        for( int i=0; i<len; ++i ) {
                            *q++ = buf[i];
                        }
                        TIXMLASSERT( q <= p );
                    }
                    else {
                        int i=0;
                        for(; i<NUM_ENTITIES; ++i ) {
                            if (    wcsncmp( p+1, entities[i].pattern, entities[i].length ) == 0
                                    && *(p+entities[i].length+1) == ';' ) {
                                // Found an entity convert;
                                *q = entities[i].value;
                                ++q;
                                p += entities[i].length + 2;
                                break;
                            }
                        }
                        if ( i == NUM_ENTITIES ) {
                            // fixme: treat as error?
                            ++p;
                            ++q;
                        }
                    }
                }
                else {
                    *q = *p;
                    ++p;
                    ++q;
                }
            }
            *q = 0;
        }
        // The loop below has plenty going on, and this
        // is a less useful mode. Break it out.
        if ( _flags & COLLAPSE_WHITESPACE ) {
            CollapseWhitespace();
        }
        _flags = (_flags & NEEDS_DELETE);
    }
    return _start;
}




// --------- XMLUtil ----------- //

const wchar_t* XMLUtil::ReadBOM( const wchar_t* p, bool* bom )
{
    *bom = false;
    const wchar_t* pu = p;
    // Check for BOM:
    if (    *(pu+0) == TIXML_UTF_LEAD_0
            && *(pu+1) == TIXML_UTF_LEAD_1
            && *(pu+2) == TIXML_UTF_LEAD_2 ) {
        *bom = true;
        p += 3;
    }
    return p;
}


void XMLUtil::ConvertUTF32ToUTF8( unsigned long input, wchar_t* output, int* length )
{
    const unsigned long BYTE_MASK = 0xBF;
    const unsigned long BYTE_MARK = 0x80;
    const unsigned long FIRST_BYTE_MARK[7] = { 0x00, 0x00, 0xC0, 0xE0, 0xF0, 0xF8, 0xFC };

    if (input < 0x80) {
        *length = 1;
    }
    else if ( input < 0x800 ) {
        *length = 2;
    }
    else if ( input < 0x10000 ) {
        *length = 3;
    }
    else if ( input < 0x200000 ) {
        *length = 4;
    }
    else {
        *length = 0;    // This code won't covert this correctly anyway.
        return;
    }

    output += *length;

    // Scary scary fall throughs.
    switch (*length) {
        case 4:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 3:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 2:
            --output;
            *output = (char)((input | BYTE_MARK) & BYTE_MASK);
            input >>= 6;
        case 1:
            --output;
            *output = (char)(input | FIRST_BYTE_MARK[*length]);
        default:
            break;
    }
}


const wchar_t* XMLUtil::GetCharacterRef( const wchar_t* p, wchar_t* value, int* length )
{
    // Presume an entity, and pull it out.
    *length = 0;

    if ( *(p+1) == '#' && *(p+2) ) {
        unsigned long ucs = 0;
        ptrdiff_t delta = 0;
        unsigned mult = 1;

        if ( *(p+2) == 'x' ) {
            // Hexadecimal.
            if ( !*(p+3) ) {
                return 0;
            }

            const wchar_t* q = p+3;
            q = wcschr( q, ';' );

            if ( !q || !*q ) {
                return 0;
            }

            delta = q-p;
            --q;

            while ( *q != 'x' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    ucs += mult * (*q - '0');
                }
                else if ( *q >= 'a' && *q <= 'f' ) {
                    ucs += mult * (*q - 'a' + 10);
                }
                else if ( *q >= 'A' && *q <= 'F' ) {
                    ucs += mult * (*q - 'A' + 10 );
                }
                else {
                    return 0;
                }
                mult *= 16;
                --q;
            }
        }
        else {
            // Decimal.
            if ( !*(p+2) ) {
                return 0;
            }

            const wchar_t* q = p+2;
            q = wcschr( q, ';' );

            if ( !q || !*q ) {
                return 0;
            }

            delta = q-p;
            --q;

            while ( *q != '#' ) {
                if ( *q >= '0' && *q <= '9' ) {
                    ucs += mult * (*q - '0');
                }
                else {
                    return 0;
                }
                mult *= 10;
                --q;
            }
        }
        // convert the UCS to UTF-8
        ConvertUTF32ToUTF8( ucs, value, length );
        return p + delta + 1;
    }
    return p+1;
}


void XMLUtil::ToStr( int v, wchar_t* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, L"%d", v );
}


void XMLUtil::ToStr( unsigned v, wchar_t* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, L"%u", v );
}


void XMLUtil::ToStr( bool v, wchar_t* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, L"%d", v ? 1 : 0 );
}


void XMLUtil::ToStr( float v, wchar_t* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, L"%g", v );
}


void XMLUtil::ToStr( double v, wchar_t* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, L"%g", v );
}

void XMLUtil::ToStr( __int64 v, wchar_t* buffer, int bufferSize )
{
    TIXML_SNPRINTF( buffer, bufferSize, L"%I64d", v );
}

bool XMLUtil::ToInt( const wchar_t* str, int* value )
{
    if ( TIXML_SSCANF( str, L"%d", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToUnsigned( const wchar_t* str, unsigned *value )
{
    if ( TIXML_SSCANF( str, L"%u", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToBool( const wchar_t* str, bool* value )
{
    int ival = 0;
    if ( ToInt( str, &ival )) {
        *value = (ival==0) ? false : true;
        return true;
    }
    if ( StringEqual( str, L"true" ) ) {
        *value = true;
        return true;
    }
    else if ( StringEqual( str, L"false" ) ) {
        *value = false;
        return true;
    }
    return false;
}


bool XMLUtil::ToFloat( const wchar_t* str, float* value )
{
    if ( TIXML_SSCANF( str, L"%f", value ) == 1 ) {
        return true;
    }
    return false;
}

bool XMLUtil::ToDouble( const wchar_t* str, double* value )
{
    if ( TIXML_SSCANF( str, L"%lf", value ) == 1 ) {
        return true;
    }
    return false;
}


wchar_t* TiXMLDocument::Identify( wchar_t* p, XMLNode** node )
{
    XMLNode* returnNode = 0;
    wchar_t* start = p;
    p = XMLUtil::SkipWhiteSpace( p );
    if( !p || !*p ) {
        return p;
    }

    // What is this thing?
    // - Elements start with a letter or underscore, but xml is reserved.
    // - Comments: <!--
    // - Decleration: <?
    // - Everthing else is unknown to tinyxml.
    //

    static const wchar_t* xmlHeader		= { L"<?" };
    static const wchar_t* commentHeader	= { L"<!--" };
    static const wchar_t* dtdHeader		= { L"<!" };
    static const wchar_t* cdataHeader		= { L"<![CDATA[" };
    static const wchar_t* elementHeader	= { L"<" };	// and a header for everything else; check last.

    static const int xmlHeaderLen		= 2;
    static const int commentHeaderLen	= 4;
    static const int dtdHeaderLen		= 2;
    static const int cdataHeaderLen		= 9;
    static const int elementHeaderLen	= 1;

#if defined(_MSC_VER)
#pragma warning ( push )
#pragma warning ( disable : 4127 )
#endif
    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLUnknown ) );		// use same memory pool
    TIXMLASSERT( sizeof( XMLComment ) == sizeof( XMLDeclaration ) );	// use same memory pool
#if defined(_MSC_VER)
#pragma warning (pop)
#endif
    if ( XMLUtil::StringEqual( p, xmlHeader, xmlHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) XMLDeclaration( this );
        returnNode->_memPool = &_commentPool;
        p += xmlHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, commentHeader, commentHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) XMLComment( this );
        returnNode->_memPool = &_commentPool;
        p += commentHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, cdataHeader, cdataHeaderLen ) ) {
        XMLText* text = new (_textPool.Alloc()) XMLText( this );
        returnNode = text;
        returnNode->_memPool = &_textPool;
        p += cdataHeaderLen;
        text->SetCData( true );
    }
    else if ( XMLUtil::StringEqual( p, dtdHeader, dtdHeaderLen ) ) {
        returnNode = new (_commentPool.Alloc()) XMLUnknown( this );
        returnNode->_memPool = &_commentPool;
        p += dtdHeaderLen;
    }
    else if ( XMLUtil::StringEqual( p, elementHeader, elementHeaderLen ) ) {
        returnNode = new (_elementPool.Alloc()) XMLElement( this );
        returnNode->_memPool = &_elementPool;
        p += elementHeaderLen;
    }
    else {
        returnNode = new (_textPool.Alloc()) XMLText( this );
        returnNode->_memPool = &_textPool;
        p = start;	// Back it up, all the text counts.
    }

    *node = returnNode;
    return p;
}


bool TiXMLDocument::Accept( XMLVisitor* visitor ) const
{
    if ( visitor->VisitEnter( *this ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- XMLNode ----------- //

XMLNode::XMLNode( TiXMLDocument* doc ) :
    _document( doc ),
    _parent( 0 ),
    _firstChild( 0 ), _lastChild( 0 ),
    _prev( 0 ), _next( 0 )
{
}


XMLNode::~XMLNode()
{
    DeleteChildren();
    if ( _parent ) {
        _parent->Unlink( this );
    }
}


void XMLNode::SetValue( const wchar_t* str, bool staticMem )
{
    if ( staticMem ) {
        _value.SetInternedStr( str );
    }
    else {
        _value.SetStr( str );
    }
}


void XMLNode::DeleteChildren()
{
    while( _firstChild ) {
        XMLNode* node = _firstChild;
        Unlink( node );

        DELETE_NODE( node );
    }
    _firstChild = _lastChild = 0;
}


void XMLNode::Unlink( XMLNode* child )
{
    TIXMLASSERT( child->_parent == this );
    if ( child == _firstChild ) {
        _firstChild = _firstChild->_next;
    }
    if ( child == _lastChild ) {
        _lastChild = _lastChild->_prev;
    }

    if ( child->_prev ) {
        child->_prev->_next = child->_next;
    }
    if ( child->_next ) {
        child->_next->_prev = child->_prev;
    }
    child->_parent = 0;
}


void XMLNode::DeleteChild( XMLNode* node )
{
    TIXMLASSERT( node->_parent == this );
    DELETE_NODE( node );
}


XMLNode* XMLNode::InsertEndChild( XMLNode* addThis )
{
    if ( _lastChild ) {
        TIXMLASSERT( _firstChild );
        TIXMLASSERT( _lastChild->_next == 0 );
        _lastChild->_next = addThis;
        addThis->_prev = _lastChild;
        _lastChild = addThis;

        addThis->_next = 0;
    }
    else {
        TIXMLASSERT( _firstChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    addThis->_memPool->SetTracked();
    return addThis;
}


XMLNode* XMLNode::InsertFirstChild( XMLNode* addThis )
{
    if ( _firstChild ) {
        TIXMLASSERT( _lastChild );
        TIXMLASSERT( _firstChild->_prev == 0 );

        _firstChild->_prev = addThis;
        addThis->_next = _firstChild;
        _firstChild = addThis;

        addThis->_prev = 0;
    }
    else {
        TIXMLASSERT( _lastChild == 0 );
        _firstChild = _lastChild = addThis;

        addThis->_prev = 0;
        addThis->_next = 0;
    }
    addThis->_parent = this;
    addThis->_memPool->SetTracked();
    return addThis;
}


XMLNode* XMLNode::InsertAfterChild( XMLNode* afterThis, XMLNode* addThis )
{
    TIXMLASSERT( afterThis->_parent == this );
    if ( afterThis->_parent != this ) {
        return 0;
    }

    if ( afterThis->_next == 0 ) {
        // The last node or the only node.
        return InsertEndChild( addThis );
    }
    addThis->_prev = afterThis;
    addThis->_next = afterThis->_next;
    afterThis->_next->_prev = addThis;
    afterThis->_next = addThis;
    addThis->_parent = this;
    addThis->_memPool->SetTracked();
    return addThis;
}




const XMLElement* XMLNode::FirstChildElement( const wchar_t* value ) const
{
    for( XMLNode* node=_firstChild; node; node=node->_next ) {
        XMLElement* element = node->ToElement();
        if ( element ) {
            if ( !value || XMLUtil::StringEqual( element->Name(), value ) ) {
                return element;
            }
        }
    }
    return 0;
}


const XMLElement* XMLNode::LastChildElement( const wchar_t* value ) const
{
    for( XMLNode* node=_lastChild; node; node=node->_prev ) {
        XMLElement* element = node->ToElement();
        if ( element ) {
            if ( !value || XMLUtil::StringEqual( element->Name(), value ) ) {
                return element;
            }
        }
    }
    return 0;
}


const XMLElement* XMLNode::NextSiblingElement( const wchar_t* value ) const
{
    for( XMLNode* element=this->_next; element; element = element->_next ) {
        if (    element->ToElement()
                && (!value || XMLUtil::StringEqual( value, element->Value() ))) {
            return element->ToElement();
        }
    }
    return 0;
}


const XMLElement* XMLNode::PreviousSiblingElement( const wchar_t* value ) const
{
    for( XMLNode* element=_prev; element; element = element->_prev ) {
        if (    element->ToElement()
                && (!value || XMLUtil::StringEqual( value, element->Value() ))) {
            return element->ToElement();
        }
    }
    return 0;
}


wchar_t* XMLNode::ParseDeep( wchar_t* p, StrPair* parentEnd )
{
    // This is a recursive method, but thinking about it "at the current level"
    // it is a pretty simple flat list:
    //		<foo/>
    //		<!-- comment -->
    //
    // With a special case:
    //		<foo>
    //		</foo>
    //		<!-- comment -->
    //
    // Where the closing element (/foo) *must* be the next thing after the opening
    // element, and the names must match. BUT the tricky bit is that the closing
    // element will be read by the child.
    //
    // 'endTag' is the end tag for this node, it is returned by a call to a child.
    // 'parentEnd' is the end tag for the parent, which is filled in and returned.

    while( p && *p ) {
        XMLNode* node = 0;

        p = _document->Identify( p, &node );
        if ( p == 0 || node == 0 ) {
            break;
        }

        StrPair endTag;
        p = node->ParseDeep( p, &endTag );
        if ( !p ) {
            DELETE_NODE( node );
            node = 0;
            if ( !_document->Error() ) {
                _document->SetError( XML_ERROR_PARSING, 0, 0 );
            }
            break;
        }

        // We read the end tag. Return it to the parent.
        if ( node->ToElement() && node->ToElement()->ClosingType() == XMLElement::CLOSING ) {
            if ( parentEnd ) {
                *parentEnd = static_cast<XMLElement*>(node)->_value;
            }
			node->_memPool->SetTracked();	// created and then immediately deleted.
            DELETE_NODE( node );
            return p;
        }

        // Handle an end tag returned to this level.
        // And handle a bunch of annoying errors.
        XMLElement* ele = node->ToElement();
        if ( ele ) {
            if ( endTag.Empty() && ele->ClosingType() == XMLElement::OPEN ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                p = 0;
            }
            else if ( !endTag.Empty() && ele->ClosingType() != XMLElement::OPEN ) {
                _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                p = 0;
            }
            else if ( !endTag.Empty() ) {
                if ( !XMLUtil::StringEqual( endTag.GetStr(), node->Value() )) {
                    _document->SetError( XML_ERROR_MISMATCHED_ELEMENT, node->Value(), 0 );
                    p = 0;
                }
            }
        }
        if ( p == 0 ) {
            DELETE_NODE( node );
            node = 0;
        }
        if ( node ) {
            this->InsertEndChild( node );
        }
    }
    return 0;
}

// --------- XMLText ---------- //
wchar_t* XMLText::ParseDeep( wchar_t* p, StrPair* )
{
    const wchar_t* start = p;
    if ( this->CData() ) {
        p = _value.ParseText( p, L"]]>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_CDATA, start, 0 );
        }
        return p;
    }
    else {
        int flags = _document->ProcessEntities() ? StrPair::TEXT_ELEMENT : StrPair::TEXT_ELEMENT_LEAVE_ENTITIES;
        if ( _document->WhitespaceMode() == COLLAPSE_WHITESPACE ) {
            flags |= StrPair::COLLAPSE_WHITESPACE;
        }

        p = _value.ParseText( p, L"<", flags );
        if ( !p ) {
            _document->SetError( XML_ERROR_PARSING_TEXT, start, 0 );
        }
        if ( p && *p ) {
            return p-1;
        }
    }
    return 0;
}


XMLNode* XMLText::ShallowClone( TiXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLText* text = doc->NewText( Value() );	// fixme: this will always allocate memory. Intern?
    text->SetCData( this->CData() );
    return text;
}


bool XMLText::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToText() && XMLUtil::StringEqual( compare->ToText()->Value(), Value() ));
}


bool XMLText::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}


// --------- XMLComment ---------- //

XMLComment::XMLComment( TiXMLDocument* doc ) : XMLNode( doc )
{
}


XMLComment::~XMLComment()
{
}


wchar_t* XMLComment::ParseDeep( wchar_t* p, StrPair* )
{
    // Comment parses as text.
    const wchar_t* start = p;
    p = _value.ParseText( p, L"-->", StrPair::COMMENT );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_COMMENT, start, 0 );
    }
    return p;
}


XMLNode* XMLComment::ShallowClone( TiXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLComment* comment = doc->NewComment( Value() );	// fixme: this will always allocate memory. Intern?
    return comment;
}


bool XMLComment::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToComment() && XMLUtil::StringEqual( compare->ToComment()->Value(), Value() ));
}


bool XMLComment::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}


// --------- XMLDeclaration ---------- //

XMLDeclaration::XMLDeclaration( TiXMLDocument* doc ) : XMLNode( doc )
{
}


XMLDeclaration::~XMLDeclaration()
{
    //printf( "~XMLDeclaration\n" );
}


wchar_t* XMLDeclaration::ParseDeep( wchar_t* p, StrPair* )
{
    // Declaration parses as text.
    const wchar_t* start = p;
    p = _value.ParseText( p, L"?>", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( p == 0 ) {
        _document->SetError( XML_ERROR_PARSING_DECLARATION, start, 0 );
    }
    return p;
}


XMLNode* XMLDeclaration::ShallowClone( TiXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLDeclaration* dec = doc->NewDeclaration( Value() );	// fixme: this will always allocate memory. Intern?
    return dec;
}


bool XMLDeclaration::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToDeclaration() && XMLUtil::StringEqual( compare->ToDeclaration()->Value(), Value() ));
}



bool XMLDeclaration::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}

// --------- XMLUnknown ---------- //

XMLUnknown::XMLUnknown( TiXMLDocument* doc ) : XMLNode( doc )
{
}


XMLUnknown::~XMLUnknown()
{
}


wchar_t* XMLUnknown::ParseDeep( wchar_t* p, StrPair* )
{
    // Unknown parses as text.
    const wchar_t* start = p;

    p = _value.ParseText( p, L">", StrPair::NEEDS_NEWLINE_NORMALIZATION );
    if ( !p ) {
        _document->SetError( XML_ERROR_PARSING_UNKNOWN, start, 0 );
    }
    return p;
}


XMLNode* XMLUnknown::ShallowClone( TiXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLUnknown* text = doc->NewUnknown( Value() );	// fixme: this will always allocate memory. Intern?
    return text;
}


bool XMLUnknown::ShallowEqual( const XMLNode* compare ) const
{
    return ( compare->ToUnknown() && XMLUtil::StringEqual( compare->ToUnknown()->Value(), Value() ));
}


bool XMLUnknown::Accept( XMLVisitor* visitor ) const
{
    return visitor->Visit( *this );
}

// --------- XMLAttribute ---------- //
wchar_t* XMLAttribute::ParseDeep( wchar_t* p, bool processEntities )
{
    // Parse using the name rules: bug fix, was using ParseText before
    p = _name.ParseName( p );
    if ( !p || !*p ) {
        return 0;
    }

    // Skip white space before =
    p = XMLUtil::SkipWhiteSpace( p );
    if ( !p || *p != '=' ) {
        return 0;
    }

    ++p;	// move up to opening quote
    p = XMLUtil::SkipWhiteSpace( p );
    if ( *p != '\"' && *p != '\'' ) {
        return 0;
    }

    wchar_t endTag[2] = { *p, 0 };
    ++p;	// move past opening quote

    p = _value.ParseText( p, endTag, processEntities ? StrPair::ATTRIBUTE_VALUE : StrPair::ATTRIBUTE_VALUE_LEAVE_ENTITIES );
    return p;
}


void XMLAttribute::SetName( const wchar_t* n )
{
    _name.SetStr( n );
}


XMLError XMLAttribute::QueryIntValue( int* value ) const
{
    if ( XMLUtil::ToInt( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryUnsignedValue( unsigned int* value ) const
{
    if ( XMLUtil::ToUnsigned( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryBoolValue( bool* value ) const
{
    if ( XMLUtil::ToBool( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryFloatValue( float* value ) const
{
    if ( XMLUtil::ToFloat( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


XMLError XMLAttribute::QueryDoubleValue( double* value ) const
{
    if ( XMLUtil::ToDouble( Value(), value )) {
        return XML_NO_ERROR;
    }
    return XML_WRONG_ATTRIBUTE_TYPE;
}


void XMLAttribute::SetAttribute( const wchar_t* v )
{
    _value.SetStr( v );
}


void XMLAttribute::SetAttribute( int v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( unsigned v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}


void XMLAttribute::SetAttribute( bool v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( double v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( float v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

void XMLAttribute::SetAttribute( __int64 v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    _value.SetStr( buf );
}

// --------- XMLElement ---------- //
XMLElement::XMLElement( TiXMLDocument* doc ) : XMLNode( doc ),
    _closingType( 0 ),
    _rootAttribute( 0 )
{
}


XMLElement::~XMLElement()
{
    while( _rootAttribute ) {
        XMLAttribute* next = _rootAttribute->_next;
        DELETE_ATTRIBUTE( _rootAttribute );
        _rootAttribute = next;
    }
}


XMLAttribute* XMLElement::FindAttribute( const wchar_t* name )
{
    XMLAttribute* a = 0;
    for( a=_rootAttribute; a; a = a->_next ) {
        if ( XMLUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}


const XMLAttribute* XMLElement::FindAttribute( const wchar_t* name ) const
{
    XMLAttribute* a = 0;
    for( a=_rootAttribute; a; a = a->_next ) {
        if ( XMLUtil::StringEqual( a->Name(), name ) ) {
            return a;
        }
    }
    return 0;
}


const wchar_t* XMLElement::Attribute( const wchar_t* name, const wchar_t* value ) const
{
    const XMLAttribute* a = FindAttribute( name );
    if ( !a ) {
        return 0;
    }
    if ( !value || XMLUtil::StringEqual( a->Value(), value )) {
        return a->Value();
    }
    return 0;
}


const wchar_t* XMLElement::GetText() const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        return FirstChild()->ToText()->Value();
    }
    return 0;
}


XMLError XMLElement::QueryIntText( int* ival ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const wchar_t* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToInt( t, ival ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryUnsignedText( unsigned* uval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const wchar_t* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToUnsigned( t, uval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryBoolText( bool* bval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const wchar_t* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToBool( t, bval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryDoubleText( double* dval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const wchar_t* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToDouble( t, dval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}


XMLError XMLElement::QueryFloatText( float* fval ) const
{
    if ( FirstChild() && FirstChild()->ToText() ) {
        const wchar_t* t = FirstChild()->ToText()->Value();
        if ( XMLUtil::ToFloat( t, fval ) ) {
            return XML_SUCCESS;
        }
        return XML_CAN_NOT_CONVERT_TEXT;
    }
    return XML_NO_TEXT_NODE;
}



XMLAttribute* XMLElement::FindOrCreateAttribute( const wchar_t* name )
{
    XMLAttribute* last = 0;
    XMLAttribute* attrib = 0;
    for( attrib = _rootAttribute;
            attrib;
            last = attrib, attrib = attrib->_next ) {
        if ( XMLUtil::StringEqual( attrib->Name(), name ) ) {
            break;
        }
    }
    if ( !attrib ) {
        attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
        attrib->_memPool = &_document->_attributePool;
        if ( last ) {
            last->_next = attrib;
        }
        else {
            _rootAttribute = attrib;
        }
        attrib->SetName( name );
        attrib->_memPool->SetTracked(); // always created and linked.
    }
    return attrib;
}


void XMLElement::DeleteAttribute( const wchar_t* name )
{
    XMLAttribute* prev = 0;
    for( XMLAttribute* a=_rootAttribute; a; a=a->_next ) {
        if ( XMLUtil::StringEqual( name, a->Name() ) ) {
            if ( prev ) {
                prev->_next = a->_next;
            }
            else {
                _rootAttribute = a->_next;
            }
            DELETE_ATTRIBUTE( a );
            break;
        }
        prev = a;
    }
}


wchar_t* XMLElement::ParseAttributes( wchar_t* p )
{
    const wchar_t* start = p;
    XMLAttribute* prevAttribute = 0;

    // Read the attributes.
    while( p ) {
        p = XMLUtil::SkipWhiteSpace( p );
        if ( !p || !(*p) ) {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, Name() );
            return 0;
        }

        // attribute.
        if ( XMLUtil::IsAlpha( *p ) ) {
            XMLAttribute* attrib = new (_document->_attributePool.Alloc() ) XMLAttribute();
            attrib->_memPool = &_document->_attributePool;
			attrib->_memPool->SetTracked();

            p = attrib->ParseDeep( p, _document->ProcessEntities() );
            if ( !p || Attribute( attrib->Name() ) ) {
                DELETE_ATTRIBUTE( attrib );
                _document->SetError( XML_ERROR_PARSING_ATTRIBUTE, start, p );
                return 0;
            }
            // There is a minor bug here: if the attribute in the source xml
            // document is duplicated, it will not be detected and the
            // attribute will be doubly added. However, tracking the 'prevAttribute'
            // avoids re-scanning the attribute list. Preferring performance for
            // now, may reconsider in the future.
            if ( prevAttribute ) {
                prevAttribute->_next = attrib;
            }
            else {
                _rootAttribute = attrib;
            }
            prevAttribute = attrib;
        }
        // end of the tag
        else if ( *p == '/' && *(p+1) == '>' ) {
            _closingType = CLOSED;
            return p+2;	// done; sealed element.
        }
        // end of the tag
        else if ( *p == '>' ) {
            ++p;
            break;
        }
        else {
            _document->SetError( XML_ERROR_PARSING_ELEMENT, start, p );
            return 0;
        }
    }
    return p;
}


//
//	<ele></ele>
//	<ele>foo<b>bar</b></ele>
//
wchar_t* XMLElement::ParseDeep( wchar_t* p, StrPair* strPair )
{
    // Read the element name.
    p = XMLUtil::SkipWhiteSpace( p );
    if ( !p ) {
        return 0;
    }

    // The closing element is the </element> form. It is
    // parsed just like a regular element then deleted from
    // the DOM.
    if ( *p == '/' ) {
        _closingType = CLOSING;
        ++p;
    }

    p = _value.ParseName( p );
    if ( _value.Empty() ) {
        return 0;
    }

    p = ParseAttributes( p );
    if ( !p || !*p || _closingType ) {
        return p;
    }

    p = XMLNode::ParseDeep( p, strPair );
    return p;
}



XMLNode* XMLElement::ShallowClone( TiXMLDocument* doc ) const
{
    if ( !doc ) {
        doc = _document;
    }
    XMLElement* element = doc->NewElement( Value() );					// fixme: this will always allocate memory. Intern?
    for( const XMLAttribute* a=FirstAttribute(); a; a=a->Next() ) {
        element->SetAttribute( a->Name(), a->Value() );					// fixme: this will always allocate memory. Intern?
    }
    return element;
}


bool XMLElement::ShallowEqual( const XMLNode* compare ) const
{
    const XMLElement* other = compare->ToElement();
    if ( other && XMLUtil::StringEqual( other->Value(), Value() )) {

        const XMLAttribute* a=FirstAttribute();
        const XMLAttribute* b=other->FirstAttribute();

        while ( a && b ) {
            if ( !XMLUtil::StringEqual( a->Value(), b->Value() ) ) {
                return false;
            }
            a = a->Next();
            b = b->Next();
        }
        if ( a || b ) {
            // different count
            return false;
        }
        return true;
    }
    return false;
}


bool XMLElement::Accept( XMLVisitor* visitor ) const
{
    if ( visitor->VisitEnter( *this, _rootAttribute ) ) {
        for ( const XMLNode* node=FirstChild(); node; node=node->NextSibling() ) {
            if ( !node->Accept( visitor ) ) {
                break;
            }
        }
    }
    return visitor->VisitExit( *this );
}


// --------- TiXMLDocument ----------- //
TiXMLDocument::TiXMLDocument( bool processEntities, Whitespace whitespace ) :
    XMLNode( 0 ),
    _writeBOM( false ),
    _processEntities( processEntities ),
    _errorID( XML_NO_ERROR ),
    _whitespace( whitespace ),
    _errorStr1( 0 ),
    _errorStr2( 0 ),
    _charBuffer( 0 )
{
    _document = this;	// avoid warning about 'this' in initializer list
}


TiXMLDocument::~TiXMLDocument()
{
    DeleteChildren();
    delete [] _charBuffer;

#if 0
    textPool.Trace( "text" );
    elementPool.Trace( "element" );
    commentPool.Trace( "comment" );
    attributePool.Trace( "attribute" );
#endif

#ifdef DEBUG
	if ( Error() == false ) {
		TIXMLASSERT( _elementPool.CurrentAllocs()   == _elementPool.Untracked() );
		TIXMLASSERT( _attributePool.CurrentAllocs() == _attributePool.Untracked() );
		TIXMLASSERT( _textPool.CurrentAllocs()      == _textPool.Untracked() );
		TIXMLASSERT( _commentPool.CurrentAllocs()   == _commentPool.Untracked() );
	}
#endif
}


void TiXMLDocument::InitDocument()
{
    _errorID = XML_NO_ERROR;
    _errorStr1 = 0;
    _errorStr2 = 0;

    delete [] _charBuffer;
    _charBuffer = 0;
}


XMLElement* TiXMLDocument::NewElement( const wchar_t* name )
{
    XMLElement* ele = new (_elementPool.Alloc()) XMLElement( this );
    ele->_memPool = &_elementPool;
    ele->SetName( name );
    return ele;
}


XMLComment* TiXMLDocument::NewComment( const wchar_t* str )
{
    XMLComment* comment = new (_commentPool.Alloc()) XMLComment( this );
    comment->_memPool = &_commentPool;
    comment->SetValue( str );
    return comment;
}


XMLText* TiXMLDocument::NewText( const wchar_t* str )
{
    XMLText* text = new (_textPool.Alloc()) XMLText( this );
    text->_memPool = &_textPool;
    text->SetValue( str );
    return text;
}


XMLDeclaration* TiXMLDocument::NewDeclaration( const wchar_t* str )
{
    XMLDeclaration* dec = new (_commentPool.Alloc()) XMLDeclaration( this );
    dec->_memPool = &_commentPool;
    dec->SetValue( str ? str : L"xml version=\"1.0\" encoding=\"UTF-8\"" );
    return dec;
}


XMLUnknown* TiXMLDocument::NewUnknown( const wchar_t* str )
{
    XMLUnknown* unk = new (_commentPool.Alloc()) XMLUnknown( this );
    unk->_memPool = &_commentPool;
    unk->SetValue( str );
    return unk;
}


XMLError TiXMLDocument::LoadFile( const wchar_t* filename )
{
    DeleteChildren();
    InitDocument();
    FILE* fp = 0;

#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
    errno_t err = _wfopen_s(&fp, filename, L"rb" );
    if ( !fp || err) {
#else
    fp = fopen( filename, "rb" );
    if ( !fp) {
#endif
        SetError( XML_ERROR_FILE_NOT_FOUND, filename, 0 );
        return _errorID;
    }
    LoadFile( fp );
    fclose( fp );
    return _errorID;
}


XMLError TiXMLDocument::LoadFile( FILE* fp )
{
    DeleteChildren();
    InitDocument();

    fseek( fp, 0, SEEK_END );
    size_t size = ftell( fp ) / sizeof(wchar_t); //because we need size in UNICODE chars not bytes
    fseek( fp, 0, SEEK_SET );

    if ( size == 0 ) {
        return _errorID;
    }

    _charBuffer = new wchar_t[size+1];
    size_t read = fread( _charBuffer, sizeof(wchar_t), size, fp );
    if ( read != size ) {
        SetError( XML_ERROR_FILE_READ_ERROR, 0, 0 );
        return _errorID;
    }

    _charBuffer[size] = 0;

    const wchar_t* p = _charBuffer;
    p = XMLUtil::SkipWhiteSpace( p );
    p = XMLUtil::ReadBOM( p, &_writeBOM );
    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    ParseDeep( _charBuffer + (p-_charBuffer), 0 );
    return _errorID;
}


XMLError TiXMLDocument::SaveFile( const wchar_t* filename, bool compact )
{
    FILE* fp = 0;
#if defined(_MSC_VER) && (_MSC_VER >= 1400 )
    errno_t err = _wfopen_s(&fp, filename, L"wt,css=UNICODE" );
    if ( !fp || err) {
#else
    fp = fopen( filename, "w" );
    if ( !fp) {
#endif
        SetError( XML_ERROR_FILE_COULD_NOT_BE_OPENED, filename, 0 );
        return _errorID;
    }
    SaveFile(fp, compact);
    fclose( fp );
    return _errorID;
}


XMLError TiXMLDocument::SaveFile( FILE* fp, bool compact )
{
    XMLPrinter stream( fp, compact );
    Print( &stream );
    return _errorID;
}


XMLError TiXMLDocument::Parse( const wchar_t *p, size_t len )
{
    DeleteChildren();
    InitDocument();

    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }
    if ( len == (size_t)(-1) ) {
        len = wcslen( p );
    }
    _charBuffer = new wchar_t[ len+1 ];
    wmemcpy(_charBuffer, p, len);
    _charBuffer[len] = 0;

    p = XMLUtil::SkipWhiteSpace( p );
    p = XMLUtil::ReadBOM( p, &_writeBOM );
    if ( !p || !*p ) {
        SetError( XML_ERROR_EMPTY_DOCUMENT, 0, 0 );
        return _errorID;
    }

    ParseDeep( _charBuffer, 0 );
    return _errorID;
}


void TiXMLDocument::Print( XMLPrinter* streamer )
{
    XMLPrinter stdStreamer( stdout );
    if ( !streamer ) {
        streamer = &stdStreamer;
    }
    Accept( streamer );
}


void TiXMLDocument::SetError( XMLError error, const wchar_t* str1, const wchar_t* str2 )
{
    _errorID = error;
    _errorStr1 = str1;
    _errorStr2 = str2;
}


void TiXMLDocument::PrintError() const
{
    if ( _errorID ) {
        static const int LEN = 20;
        wchar_t buf1[LEN] = { 0 };
        wchar_t buf2[LEN] = { 0 };

        if ( _errorStr1 ) {
            TIXML_SNPRINTF( buf1, LEN, L"%s", _errorStr1 );
        }
        if ( _errorStr2 ) {
            TIXML_SNPRINTF( buf2, LEN, L"%s", _errorStr2 );
        }

        wprintf( L"TiXMLDocument error id=%d str1=%s str2=%s\n",
                _errorID, buf1, buf2 );
    }
}


XMLPrinter::XMLPrinter( FILE* file, bool compact ) :
    _elementJustOpened( false ),
    _firstElement( true ),
    _fp( file ),
    _depth( 0 ),
    _textDepth( -1 ),
    _processEntities( true ),
    _compactMode( compact )
{
    for( int i=0; i<ENTITY_RANGE; ++i ) {
        _entityFlag[i] = false;
        _restrictedEntityFlag[i] = false;
    }
    for( int i=0; i<NUM_ENTITIES; ++i ) {
        TIXMLASSERT( entities[i].value < ENTITY_RANGE );
        if ( entities[i].value < ENTITY_RANGE ) {
            _entityFlag[ (int)entities[i].value ] = true;
        }
    }
    _restrictedEntityFlag[(int)'&'] = true;
    _restrictedEntityFlag[(int)'<'] = true;
    _restrictedEntityFlag[(int)'>'] = true;	// not required, but consistency is nice
    _buffer.Push( 0 );
}


void XMLPrinter::Print( const wchar_t* format, ... )
{
    va_list     va;
    va_start( va, format );

    if ( _fp ) {
        vfwprintf( _fp, format, va );
    }
    else {
        // This seems brutally complex. Haven't figured out a better
        // way on windows.
#ifdef _MSC_VER
        int len = -1;
        int expand = 1000;
        while ( len < 0 ) {
            len = _vsnwprintf_s( _accumulator.Mem(), _accumulator.Capacity(), _TRUNCATE, format, va );
            if ( len < 0 ) {
                expand *= 3/2;
                _accumulator.PushArr( expand );
            }
        }
        wchar_t* p = _buffer.PushArr( len ) - 1;
        wmemcpy( p, _accumulator.Mem(), len+1 );
#else
        int len = vsnprintf( 0, 0, format, va );
        // Close out and re-start the va-args
        va_end( va );
        va_start( va, format );
        char* p = _buffer.PushArr( len ) - 1;
        vsnprintf( p, len+1, format, va );
#endif
    }
    va_end( va );
}


void XMLPrinter::PrintSpace( int depth )
{
    for( int i=0; i<depth; ++i ) {
        Print( L"    " );
    }
}


void XMLPrinter::PrintString( const wchar_t* p, bool restricted )
{
    // Look for runs of bytes between entities to print.
    const wchar_t* q = p;
    const bool* flag = restricted ? _restrictedEntityFlag : _entityFlag;

    if ( _processEntities ) {
        while ( *q ) {
            // Remember, char is sometimes signed. (How many times has that bitten me?)
            if ( *q > 0 && *q < ENTITY_RANGE ) {
                // Check for entities. If one is found, flush
                // the stream up until the entity, write the
                // entity, and keep looking.
                if ( flag[(unsigned)(*q)] ) {
                    while ( p < q ) {
                        Print( L"%c", *p );
                        ++p;
                    }
                    for( int i=0; i<NUM_ENTITIES; ++i ) {
                        if ( entities[i].value == *q ) {
                            Print( L"&%s;", entities[i].pattern );
                            break;
                        }
                    }
                    ++p;
                }
            }
            ++q;
        }
    }
    // Flush the remaining string. This will be the entire
    // string if an entity wasn't found.
    if ( !_processEntities || (q-p > 0) ) {
        Print( L"%s", p );
    }
}


void XMLPrinter::PushHeader( bool writeBOM, bool writeDec )
{
    static const unsigned char bom[] = { TIXML_UTF_LEAD_0, TIXML_UTF_LEAD_1, TIXML_UTF_LEAD_2, 0 };
    if ( writeBOM ) {
        Print( L"%s", bom );
    }
    if ( writeDec ) {
        PushDeclaration( L"xml version=\"1.0\"" );
    }
}


void XMLPrinter::OpenElement( const wchar_t* name )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    _stack.Push( name );

    if ( _textDepth < 0 && !_firstElement && !_compactMode ) {
        Print( L"\n" );
        PrintSpace( _depth );
    }

    Print( L"<%s", name );
    _elementJustOpened = true;
    _firstElement = false;
    ++_depth;
}


void XMLPrinter::PushAttribute( const wchar_t* name, const wchar_t* value )
{
    TIXMLASSERT( _elementJustOpened );
    Print( L" %s=\"", name );
    PrintString( value, false );
    Print( L"\"" );
}


void XMLPrinter::PushAttribute( const wchar_t* name, int v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const wchar_t* name, unsigned v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const wchar_t* name, bool v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::PushAttribute( const wchar_t* name, double v )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( v, buf, BUF_SIZE );
    PushAttribute( name, buf );
}


void XMLPrinter::CloseElement()
{
    --_depth;
    const wchar_t* name = _stack.Pop();

    if ( _elementJustOpened ) {
        Print( L"/>" );
    }
    else {
        if ( _textDepth < 0 && !_compactMode) {
            Print( L"\n" );
            PrintSpace( _depth );
        }
        Print( L"</%s>", name );
    }

    if ( _textDepth == _depth ) {
        _textDepth = -1;
    }
    if ( _depth == 0 && !_compactMode) {
        Print( L"\n" );
    }
    _elementJustOpened = false;
}


void XMLPrinter::SealElement()
{
    _elementJustOpened = false;
    Print( L">" );
}


void XMLPrinter::PushText( const wchar_t* text, bool cdata )
{
    _textDepth = _depth-1;

    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( cdata ) {
        Print( L"<![CDATA[" );
        Print( L"%s", text );
        Print( L"]]>" );
    }
    else {
        PrintString( text, true );
    }
}

void XMLPrinter::PushText( int value )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( unsigned value )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( bool value )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( float value )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushText( double value )
{
    wchar_t buf[BUF_SIZE];
    XMLUtil::ToStr( value, buf, BUF_SIZE );
    PushText( buf, false );
}


void XMLPrinter::PushComment( const wchar_t* comment )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( L"\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( L"<!--%s-->", comment );
}


void XMLPrinter::PushDeclaration( const wchar_t* value )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( L"\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( L"<?%s?>", value );
}


void XMLPrinter::PushUnknown( const wchar_t* value )
{
    if ( _elementJustOpened ) {
        SealElement();
    }
    if ( _textDepth < 0 && !_firstElement && !_compactMode) {
        Print( L"\n" );
        PrintSpace( _depth );
    }
    _firstElement = false;
    Print( L"<!%s>", value );
}


bool XMLPrinter::VisitEnter( const TiXMLDocument& doc )
{
    _processEntities = doc.ProcessEntities();
    if ( doc.HasBOM() ) {
        PushHeader( true, false );
    }
    return true;
}


bool XMLPrinter::VisitEnter( const XMLElement& element, const XMLAttribute* attribute )
{
    OpenElement( element.Name() );
    while ( attribute ) {
        PushAttribute( attribute->Name(), attribute->Value() );
        attribute = attribute->Next();
    }
    return true;
}


bool XMLPrinter::VisitExit( const XMLElement& )
{
    CloseElement();
    return true;
}


bool XMLPrinter::Visit( const XMLText& text )
{
    PushText( text.Value(), text.CData() );
    return true;
}


bool XMLPrinter::Visit( const XMLComment& comment )
{
    PushComment( comment.Value() );
    return true;
}

bool XMLPrinter::Visit( const XMLDeclaration& declaration )
{
    PushDeclaration( declaration.Value() );
    return true;
}


bool XMLPrinter::Visit( const XMLUnknown& unknown )
{
    PushUnknown( unknown.Value() );
    return true;
}

}   // namespace tinyxml2