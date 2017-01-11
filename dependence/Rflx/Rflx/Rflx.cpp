#include "Rflx.h"
#include <sstream>
#ifdef _MSC_VER
	#pragma warning( disable : 4996 )
#endif

namespace rflx
{
	static inline bool _isSpace( char c ) { return c == ' ' || c == '\t' || c == '\n' || c == '\r'; }

	static char* _strtrim( char* str )
	{
		const char* cur = str;
		int dst = 0;
		while ( *cur ) {
			if ( !_isSpace( *cur ) )
				str[ dst++ ] = *cur;
			++cur;
		}
		str[ dst ] = 0;
		return str;
	}

	namespace _internal
	{
		const DataTypeSizeName GValueDataTypeSizeNameTable[ vdt_max_num ] = 
		{
			{ 0, vdt_nil, "<nil>" },
			{ 0, vdt_reflexable_begin, "<reflexable_begin>" },
			{ 0, vdt_base_type_begin, "<base_type_begin>" },
					{ sizeof(bool), vdt_bool, "bool" },
					{ 0, vdt_number_begin, "<number_begin>" },
						{ 0, vdt_integer_number_begin, "<integer_number_begin>" },
							{ 0, vdt_signed_integer_number_begin, "<signed_integer_number_begin>" },
								{ sizeof(char), vdt_char, "char" },
								{ sizeof(short), vdt_short, "short" },
								{ sizeof(int), vdt_int, "int" },
								{ sizeof(long), vdt_long, "long" },
								{ sizeof(long long), vdt_llong, "long long" },
							{ 0, vdt_signed_integer_number_end, "<signed_integer_number_end>" },
							{ 0, vdt_unsigned_integer_number_begin, "<unsigned_integer_number_begin>" },
								{ sizeof(wchar_t), vdt_wchar, "wchar_t" },
								{ sizeof(unsigned char), vdt_uchar, "unsigned char" },
								{ sizeof(unsigned short), vdt_ushort, "unsigned short" },
								{ sizeof(unsigned int), vdt_uint, "unsigned int" },
								{ sizeof(unsigned long), vdt_ulong, "unsigned long" },
								{ sizeof(unsigned long long), vdt_ullong, "unsigned long long" },
							{ 0, vdt_unsigned_integer_number_end, "<unsigned_integer_number_end>" },
						{ 0, vdt_integer_number_end, "<integer_number_end>" },
						{ 0, vdt_float_number_begin, "<float_number_begin>" },
							{ sizeof(float), vdt_float, "float" },
							{ sizeof(double), vdt_double, "double" },
						{ 0, vdt_float_number_end, "<float_number_end>" },
					{ 0, vdt_number_end, "<number_end>" },
				{ 0, vdt_base_type_end, "<base_type_end>" },
				{ 0, vdt_string, "string" },
				{ 0, vdt_wstring, "wstring" },
				{ 0, vdt_enum, "enum" },
				{ 0, vdt_custom, "custom" },
				{ sizeof(const void*), vdt_pointer, "pointer" },
			{ 0, vdt_reflexable_end, "<reflexable_end>" },
		};
	}

	ValueData::ValueData( const std::string& value )
	{
		type = vdt_string;
		size_t count = value.length() + 1;
		char* p = ( char* )malloc( sizeof( char ) * count );
		std::char_traits< char >::copy( p, value.c_str(), count );
		_string = p;
	}

	ValueData::ValueData( const std::wstring& value )
	{
		type = vdt_wstring;
		size_t count = value.length() + 1;
		wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
		if ( p )
			std::char_traits< wchar_t >::copy( p, value.c_str(), count );
		_wstring = p;
	}

	ValueData::ValueData( const char* value ) : type( vdt_nil )
	{
		if ( value ) {
			size_t count = std::char_traits< char >::length( value ) + 1;
			char* p = ( char* )malloc( sizeof( char ) * count );
			if ( p )
				std::char_traits< char >::copy( p, value, count );
			_string = p;
			type = vdt_string;
		}
	}

	ValueData::ValueData( const wchar_t* value ) : type( vdt_nil )
	{
		if ( value ) {
			size_t count = std::char_traits< wchar_t >::length( value ) + 1;
			wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
			if ( p )
				std::char_traits< wchar_t >::copy( p, value, count );
			_wstring = p;
			type = vdt_wstring;
		}
	}

	ValueData::ValueData( const void* ptr ) : type( vdt_pointer ), _pointer( ptr )
	{
	}

	ValueData::ValueData( const void* custom, const CustomDataHandler* handler ) : type( vdt_nil )
	{
		if ( custom && handler ) {
			_customHandler = ( CustomDataHandler* )::malloc( sizeof( CustomDataHandler ) );
			if ( _customHandler )
				memcpy( _customHandler, handler, sizeof( CustomDataHandler ) );
			_custom = handler->clone( custom );
			type = vdt_custom;
		}
	}

	ValueData::~ValueData( )
	{
		clear();
	}

	ValueData::ValueData( const ValueData& other ) : type( vdt_nil )
	{
		copy( other );
	}

	void ValueData::clear( )
	{
		switch( type ) {
			case vdt_string:
				::free( _string );
				_string = NULL;
				break;
			case vdt_wstring:
				::free( _wstring );
				_wstring = NULL;
				break;
			case vdt_enum:
				_enumInfo  = NULL;
				break;
			case vdt_custom:
				if ( _customHandler )
					_customHandler->destroy( _custom );
				_custom = NULL;
				::free( _customHandler );
				_customHandler = NULL;
				break;
		};
		type = vdt_nil;
	}

	ValueData& ValueData::swap( ValueData& other ) {
		if ( this != &other ) {
			char m[ sizeof( *this ) ];
			memcpy( m, &other, sizeof( *this ) );
			memcpy( &other, this, sizeof( *this ) );
			memcpy( this, m, sizeof( *this ) );
		}
		return *this;
	}

	ValueData& ValueData::copy( const ValueData& other )
	{
		if ( this == &other )
			return *this;
		clear();
		type = other.type;
		switch ( type ) {
		case vdt_string:
			{
				size_t count = std::char_traits< char >::length( other._string ) + 1;
				char* p = ( char* )malloc( sizeof( char ) * count );
				if ( p ) {
					std::char_traits< char >::copy( p, other._string, count );
					_string = p;
				} else _string = NULL;
			}
			break;
		case vdt_wstring:
			{
				size_t count = std::char_traits< wchar_t >::length( other._wstring ) + 1;
				wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
				if ( p ) {
					std::char_traits< wchar_t >::copy( p, other._wstring, count );
					_wstring = p;
				} else _wstring = NULL;
			}
			break;
		case vdt_enum:
			{
				_enum = other._enum;
				_enumInfo = other._enumInfo;
			}
			break;
		case vdt_custom:
			{
				_custom = NULL;
				if ( other._custom && other._customHandler )
					_custom = other._customHandler->clone( other._custom );
				_customHandler = NULL;
				if ( other._customHandler ) {
					_customHandler = ( CustomDataHandler* )::malloc( sizeof( CustomDataHandler ) );
					if ( _customHandler )
						memcpy( _customHandler, other._customHandler, sizeof( CustomDataHandler ) );
				}
			}
			break;
		default:;
			size_t size = getValueDataTypeSize( other.type );
			memcpy( &_bool, &other._bool, size );
		}
		return *this;
	}

	bool ValueData::operator == ( const ValueData& other )
	{		
		if ( this == &other )
			return true;
		if ( type != other.type || type == vdt_nil || other.type == vdt_nil )
			return false;
		switch ( type ) {
		case vdt_string:
			return strcmp( _string, other._string ) == 0;
		case vdt_wstring:
			return wcscmp( _wstring, other._wstring ) == 0;	
		case vdt_enum:
			return _enum == other._enum && _enumInfo == other._enumInfo && _enumInfo != NULL;
		case vdt_custom:
			{
				if ( _customHandler && other._customHandler ) {
					if ( _customHandler->name() == other._customHandler->name() || strcmp( _customHandler->name(), other._customHandler->name() ) == 0 ) {
						return _custom && other._custom && _customHandler->equal( _custom, other._custom ) &&
							other._customHandler->equal( _custom, other._custom );
					}
				}
				return false;
			}
			break;
		default:;
			size_t size = getValueDataTypeSize( other.type );
			return memcmp( &_bool, &other._bool, size ) == 0;
		}
	}

	ValueData& ValueData::operator = ( const char* value )
	{
		if ( value ) {
			clear();
			size_t count = std::char_traits< char >::length( value ) + 1;
			char* p = ( char* )malloc( sizeof( char ) * count );
			if ( p )
				std::char_traits< char >::copy( p, value, count );
			_string = p;
			type = vdt_string;
		}
		return *this;
	}

	ValueData& ValueData::operator = ( const wchar_t* value )
	{	
		if ( value ) {
			clear();
			size_t count = std::char_traits< wchar_t >::length( value ) + 1;
			wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
			if ( p )
				std::char_traits< wchar_t >::copy( p, value, count );
			_wstring = p;
			type = vdt_wstring;
		}
		return *this;
	}

	ValueData& ValueData::operator = ( const std::string& value )
	{		
		if ( !value.empty() ) {
			clear();
			size_t count = value.length() + 1;
			char* p = ( char* )malloc( sizeof( char ) * count );
			if ( p )
				std::char_traits< char >::copy( p, value.c_str(), count );
			_string = p;
			type = vdt_string;
		}
		return *this;
	}

	ValueData& ValueData::operator = ( const std::wstring& value )
	{
		if ( !value.empty() ) {
			clear();
			size_t count = value.length() + 1;
			wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
			if ( p )
				std::char_traits< wchar_t >::copy( p, value.c_str(), count );
			_wstring = p;
			type = vdt_wstring;
		}
		return *this;
	}

	ValueData& ValueData::operator = ( const void* value )
	{
		clear();
		type = vdt_pointer;
		_pointer = value;
		return *this;
	}

	ValueData& ValueData::assign( const std::string& value )
	{
		clear();
		size_t count = value.length() + 1;
		char* p = ( char* )malloc( sizeof( char ) * count );
		if ( p )
			std::char_traits< char >::copy( p, value.c_str(), count );
		_string = p;
		type = vdt_string;
		return *this;
	}

	ValueData& ValueData::assign( const std::wstring& value )
	{
		clear();
		size_t count = value.length() + 1;
		wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
		if ( p )
			std::char_traits< wchar_t >::copy( p, value.c_str(), count );
		_wstring = p;
		type = vdt_wstring;
		return *this;
	}

	ValueData& ValueData::assign( const char* value )
	{
		clear();
		if ( value ) {
			size_t count = std::char_traits< char >::length( value ) + 1;
			char* p = ( char* )malloc( sizeof( char ) * count );
			if ( p )
				std::char_traits< char >::copy( p, value, count );
			_string = p;
			type = vdt_string;
		}
		return *this;
	}

	ValueData& ValueData::assign( const wchar_t* value )
	{
		clear();
		if ( value ) {
			size_t count = std::char_traits< wchar_t >::length( value ) + 1;
			wchar_t* p = ( wchar_t* )malloc( sizeof( wchar_t ) * count );
			if ( p )
				std::char_traits< wchar_t >::copy( p, value, count );
			_wstring = p;
			type = vdt_wstring;
		}
		return *this;
	}

	ValueData& ValueData::assign( const void* custom, const CustomDataHandler* handler )
	{
		clear();
		if ( custom && handler ) {
			_customHandler = ( CustomDataHandler* )::malloc( sizeof( CustomDataHandler ) );
			if ( _customHandler )
				memcpy( _customHandler, handler, sizeof( CustomDataHandler ) );
			_custom = handler->clone( custom );
			type = vdt_custom;
		}
		return *this;
	}

	ValueData& ValueData::assign( const void* value )
	{		
		clear();
		type = vdt_pointer;
		_pointer = value;
		return *this;
	}

	bool ValueData::toString( std::string& result ) const
	{
		result.clear();
		std::stringstream interpreter;
		switch ( type ) {
		case vdt_bool:
			interpreter << _bool ? "true" : "false";
			break;
		case vdt_char:			
			interpreter << _char;
			break;
		case vdt_short:			
			interpreter << _short;
			break;
		case vdt_int:			
			interpreter << _int;
			break;
		case vdt_long:			
			interpreter << _long;
			break;
		case vdt_llong:			
			interpreter << _llong;
			break;
		case vdt_wchar:			
			interpreter << (int)_wchar;
			break;
		case vdt_uchar:			
			interpreter << _uchar;
			break;
		case vdt_ushort:			
			interpreter << _ushort;
			break;
		case vdt_uint:			
			interpreter << _uint;
			break;
		case vdt_ulong:			
			interpreter << _ulong;
			break;
		case vdt_ullong:			
			interpreter << _ullong;
			break;
		case vdt_float:			
			interpreter << _float;
			break;
		case vdt_double:			
			interpreter << _double;
			break;
		case vdt_string:
			{
				if ( _string )
					result = _string;
				return true;
			}
			break;
		case vdt_wstring:
			{
				const wchar_t* c = _wstring;
				while ( *c )
					result.push_back( (char)*c++ );
				return true;
			}
			break;
		case vdt_enum:
			interpreter << _enumInfo->name;
			interpreter << _enum;
			break;
		case vdt_pointer:			
			interpreter << _pointer;
			break;
		default: return false;
		}
		if ( !interpreter || !( interpreter >> result ) || !( interpreter >> std::ws ).eof() )
			return false;
		return true;
	}

	bool ValueData::fromString( ValueDataType targetType, const std::string& source )
	{
		ValueData temp;
		std::stringstream interpreter;
		if ( targetType != vdt_string && targetType != vdt_wstring ) 	{
			interpreter << source;
			if ( !interpreter )
				return false;
		}
		temp.type = targetType;
		switch ( targetType ) {
		case vdt_bool:
			if ( !isdigit( source[0] ) )
			{
				std::string _s = source;
				std::transform( _s.begin(), _s.end(), _s.begin(), (int(*)(int))::tolower );
				std::string::iterator it = std::remove_if( _s.begin(), _s.end(), _isSpace );
				_s.erase( it, _s.end() );
				if ( _s == "true" )
					temp._bool = true;
				else
					temp._bool = false;
			}
			else
				interpreter >> temp._bool;
			break;
		case vdt_char:
			interpreter >> temp._char;
			break;
		case vdt_short:
			interpreter >> temp._short;
			break;
		case vdt_int:
			interpreter >> temp._int;
			break;
		case vdt_long:
			interpreter >> temp._long;
			break;
		case vdt_llong:
			interpreter >> temp._llong;
			break;
		case vdt_wchar:
			int wchar;
			interpreter >> wchar;
			temp._wchar = (wchar_t)wchar;
			break;
		case vdt_uchar:
			interpreter >> temp._uchar;
			break;
		case vdt_ushort:
			interpreter >> temp._ushort;
			break;
		case vdt_uint:
			interpreter >> temp._uint;
			break;
		case vdt_ulong:
			interpreter >> temp._ulong;
			break;
		case vdt_ullong:
			interpreter >> temp._ullong;
			break;
		case vdt_float:
			interpreter >> temp._float;
			break;
		case vdt_double:
			interpreter >> temp._double;
			break;
		case vdt_string:
			{
				temp.type = vdt_nil;
				temp.assign( source );
				swap( temp );
				return true;
			}
			break;
		case vdt_wstring:
			{
				std::wstring _ws;
				const char* c = source.c_str();
				while ( *c )
					_ws.push_back( *c++ );
				temp.type = vdt_nil;
				temp.assign( _ws );
				swap( temp );
				return true;
			}
			break;
		case vdt_enum:
			{
				std::string enumInfoName;
				interpreter >> enumInfoName;
				temp._enumInfo = findEnumInfo( enumInfoName.c_str() );
				RFLX_DASSERT( temp._enumInfo );
				interpreter >> temp._enum;
			}
			break;
		case vdt_pointer:
			intptr_t pointer;
			interpreter >> pointer;
			temp._pointer = ( const void* )pointer;
			break;
		default: return false;
		}
		if ( !( interpreter >> std::ws ).eof() )
			return false;
		swap( temp );
		return true;
	}

	ValueData ValueData::cast( ValueDataType targetType ) const
	{
		if ( type == targetType )
			return *this;
		ValueData result;
		std::string stream;
		if ( toString( stream ) )
			result.fromString( targetType, stream );
		return result;
	}

	struct DVStack
	{
		size_t maxSize;
		std::vector< ValueData > stack;
	};

	namespace DValueStack
	{
		static DVStack GValueStack;

		DVStack* create( size_t maxSize )
		{
			DVStack* stack = new DVStack;
			stack->maxSize = maxSize;
			return stack;
		}

		void destroy( DVStack* stack )
		{
			if ( stack == &GValueStack )
				return;
			delete stack;
		}

		DVStack& getGlobal()
		{
			return GValueStack;
		}

		size_t maxSize( const DVStack& stack )
		{
			return stack.maxSize;
		}

		size_t size( const DVStack& stack )
		{
			return stack.stack.size();
		}

		ValueData& getAt( DVStack& stack, int index )
		{
			if ( index > 0 ) {
				RFLX_DASSERT( index <= (int)stack.stack.size() );
				return stack.stack[ index - 1 ];
			} else if ( index < 0 ) {
				RFLX_DASSERT( -index <= (int)stack.stack.size() );
				return stack.stack[ stack.stack.size() + index ];
			}
			return stack.stack[0];
		}

		const ValueData& getAt( const DVStack& stack, int index )
		{
			return getAt( const_cast< DVStack& >( stack ), index );
		}

		void pop( DVStack& stack )
		{
			if ( !stack.stack.empty() )
				stack.stack.pop_back();
		}

		void push( DVStack& stack, const ValueData& value )
		{
			if ( stack.stack.size() < stack.maxSize )
				stack.stack.push_back( value );
			else
				RFLX_DASSERT( 0 && "DVStack is overflow!" );
		}

		ValueData popData( DVStack& stack )
		{
			if ( stack.stack.empty() )
				return ValueData();
			else {
				ValueData result = stack.stack.back();
				stack.stack.pop_back();
				return result;
			}
		}
	}

	namespace _internal
	{
		template< typename T > struct deleter {
			inline void operator()( T*& p ){ delete p; p = NULL; }
			inline void operator()( T& p ){ delete p; p = NULL; }
		};

		template< typename Key, typename T > struct map_deleter {
			typedef std::pair< Key, T > value_type;
			inline void operator()( const value_type& p ) { delete p.second; }
		};

		struct PropDefSorter {
			inline bool operator()( const PropDef* _left, const PropDef* _right ) const {
				return strcmp( _left->name, _right->name ) < 0;
			}
		};

		const char* _safeString( const char* str ) {
			static char _s[] = "";
			return str ? str : _s;
		}
		
		struct _PropGroup {
			std::string groupName;
			std::vector< const PropDef* > props;
			_PropGroup( const std::string& name ) : groupName( name ){}
		};

		struct _ClassInfo {
			std::string className;
			std::string baseClassName;
			unsigned int flags;
			ObjectMethods methods;
			const PropDef* propDefs;
			unsigned int propCount;
			Klass* outputAddress;
			_ClassInfo( const ClassInfo& src, Klass* output ) : outputAddress( output ) {
				className = _safeString( src.className );
				baseClassName = _safeString( src.baseClassName );
				flags = src.flags;
				methods = src.methods;
				propDefs = src.propDefs;
				propCount = src.propCount;
			}
			void convertTo( ClassInfo& dst ) const {
				dst.className = className.c_str();
				dst.baseClassName = baseClassName.c_str();
				dst.flags = flags;
				dst.methods = methods;
				dst.propDefs = propDefs;
				dst.propCount = propCount;
			}
		};

		struct _RflxClass;
		struct _ParentNode
		{
			const _RflxClass* klass;
			size_t offset;
			_ParentNode( const _RflxClass* _klass ) : klass( _klass ), offset( 0 ) {}
			bool operator < ( const _ParentNode& right ) const { return klass < right.klass; }
			bool operator == ( const _ParentNode& right ) const { return klass == right.klass; }
			operator const _RflxClass* () const { return klass; }
		};

		struct _RflxClass
		{
			// do not change flags declaration position in this struct.
			unsigned int flags;
			size_t baseOffset;
			std::string className;
			std::string baseClassName;
			ObjectMethods methods;
			_RflxClass* parent;
			std::vector< _RflxClass* > children;
			std::set< _ParentNode > parentsLookup;
			std::map< std::string, _PropGroup* > propGroup;
			PropDef* propDefs;
			std::vector< const PropDef* > sortedPropDefs;
			unsigned int propCount;
			unsigned int instanceCount;
			bool isRoot( ) const { return parent == NULL; }
			bool isLeaf( ) const { return children.empty(); }
			_RflxClass( ) : parent( NULL ), baseOffset( 0 ), propDefs( NULL ), propCount( 0 ), instanceCount( 0 )
			{}
			~_RflxClass( )
			{
				delete [] propDefs;
				std::for_each( propGroup.begin(), propGroup.end(), map_deleter< std::string, _PropGroup* >() );
				RFLX_DASSERT( instanceCount == 0 );
			}
			inline unsigned int decInstCount() { RFLX_DASSERT( instanceCount > 0 ); --instanceCount; return instanceCount; }
			inline unsigned int incInstCount() { ++instanceCount; return instanceCount; }
			static void initLookup( _RflxClass* _class, std::set< _ParentNode >* lookup = NULL )
			{
				if ( !lookup ) {
					lookup = &_class->parentsLookup;
					if ( !lookup->empty() )
						return;
				}
				if ( _class->parent ) {
					lookup->insert( _class->parent );
					initLookup( _class->parent, lookup );
				}
			}
			static void initProps( _RflxClass* _class )
			{
				if ( !_class->propCount || !_class->propGroup.empty() )
					return;
				for ( unsigned int i = 0; i < _class->propCount; ++i ) {
					const PropDef& def = _class->propDefs[ i ];
					std::string groupName = _safeString( def.groupName );
					std::map< std::string, _PropGroup* >::iterator it = _class->propGroup.find( groupName );
					if ( it == _class->propGroup.end() )
						it = _class->propGroup.insert( std::make_pair( groupName, new _PropGroup( groupName ) ) ).first;
					it->second->props.push_back( &def );
				}
			}
		};

		static inline _RflxClass* _handle2Class( Klass klass )
		{
			return reinterpret_cast< _RflxClass* >( klass );
		}

		static inline Klass _class2Handle( const _RflxClass* _class )
		{
			return reinterpret_cast< Klass >( const_cast< _RflxClass* >( _class ) );
		}

		unsigned int _addClassInstanceCount( Klass klass )
		{
			return _handle2Class( klass )->incInstCount();
		}

		unsigned int _releaseClassInstanceCount( Klass klass )
		{
			return _handle2Class( klass )->decInstCount();
		}

		typedef std::map< std::string, _RflxClass* > RflxClassBank;
		typedef std::map< std::string, EnumInfo* > EnumBank;
		struct Context {
			RflxClassBank classBank;
			EnumBank enumBank;
			bool initialized;
			int autoRegisteringFlag;
			Klass* autoRegisterClassHandleAddress;
			std::vector< _ClassInfo* > autoRegisterClassCache;
			Context() : initialized( false ), autoRegisteringFlag( 0 ) { }
			~Context()
			{ std::for_each( autoRegisterClassCache.begin(), autoRegisterClassCache.end(), deleter< _ClassInfo >() ); }
			void doAutoRegister() throw();
			void addAutoRegisterInfo( const ClassInfo& classInfo, Klass* outputAddress )
			{ autoRegisterClassCache.push_back( new _ClassInfo( classInfo, outputAddress ) ); }
		};
		static Context& _getContext() {
			static Context* _context = NULL;
			if ( _context == NULL )
				_context = new Context;
			return *_context;
		}

		static ErrorCode _unregisterClass( _RflxClass* _class )
		{
			RFLX_DASSERT( _class );
			RflxClassBank::iterator it = _getContext().classBank.find( _class->className );
			if ( it == _getContext().classBank.end() )
				return err_class_not_found;
			// class can't be release util its child have been released.
			if ( !_class->children.empty() || _class->instanceCount != 0 )
				return err_class_is_being_used;
			if ( _class->parent ) {
				_RflxClass* parent = _class->parent;
				parent->children.erase( std::find( parent->children.begin(), parent->children.end(), _class ) );
			}
			_getContext().classBank.erase( it );
			delete _class;
			return err_ok;
		}

		static ErrorCode _unregisterRootClass( _RflxClass* _class )
		{
			while ( !_class->children.empty() ) {
				ErrorCode err = _unregisterRootClass( _class->children.back() );
				if ( err != err_ok ) {
					RFLX_DASSERT( 0 && "_unregisterRootClass failed!" );
					return err;
				}
			}
			RFLX_DASSERT( _class->isLeaf() );
			return _unregisterClass( _class );
		}

		static ErrorCode _registerBaseObject() {
			RflxObjectName< Rflxable >::name( "Rflxable" );
			RflxObjectName< RflxObject >::name( "RflxObject" );
			if ( Rflxable::_class() || RflxObject::_class() ) 
				return err_ok;
			ErrorCode err = err_ok;
			{
				ClassInfo ci;
				Klass& klass = const_cast< Klass& >( Rflxable::_class() );
				memset( &ci, 0, sizeof( ci ) );
				ci.baseClassName = NULL;
				ci.className = Rflxable::_name();
				err = registerClass( &ci, &klass );
				if ( err == err_class_is_already_registered )
					err = err_ok;
				_RflxClass* _class = _handle2Class( Rflxable::_class() );
				_class->baseOffset = 0;
			}	
			{
				ClassInfo ci;
				Klass& klass = const_cast< Klass& >( RflxObject::_class() );
				memset( &ci, 0, sizeof( ci ) );
				ci.baseClassName = Rflxable::_name();
				ci.className = RflxObject::_name();
				err = registerClass( &ci, &klass );
				if ( err == err_class_is_already_registered )
					err = err_ok;
				_RflxClass* _class = _handle2Class( RflxObject::_class() );
				_class->baseOffset = 0;
			}
			return err;
		}

		const EnumInfo* _addEnumInfo( const char* name, const EnumValue* values, unsigned int count )
		{
			RFLX_DASSERT( name && values );
			std::string _name( name );
			EnumBank::iterator it = _getContext().enumBank.find( _name );
			if ( _getContext().enumBank.end() != it )
				return it->second;
			return _getContext().enumBank.insert( std::make_pair( _name, new EnumInfo( name, values, count ) ) ).first->second;
		}

		void Context::doAutoRegister() throw()
		{
			for ( ;!autoRegisterClassCache.empty(); ) {
				std::vector< _ClassInfo* >::iterator it = autoRegisterClassCache.begin();
				while ( it != autoRegisterClassCache.end() ) {
					const _ClassInfo* classInfo = *it;
					ClassInfo ci;
					classInfo->convertTo( ci );
					Klass& klass = *classInfo->outputAddress;
					if ( classInfo->baseClassName.empty() ) {
						if ( err_ok != registerClass( &ci, &klass ) ) {
							RFLX_DASSERT( 0 && "doAutoRegister registerClass failed!" );
						}
						delete classInfo;
						it = autoRegisterClassCache.erase( it );
					} else {
						ErrorCode err = registerClass( &ci, &klass );
						if ( err == err_ok || err == err_class_is_already_registered ) {
							delete classInfo;
							it = autoRegisterClassCache.erase( it );
						} else if ( err == err_base_class_not_found ) {
							++it;
						} else {
							RFLX_DASSERT( 0 && "doaAutoRegister registerClass failed!" );
						}
					}
				}
			}
			std::for_each( autoRegisterClassCache.begin(), autoRegisterClassCache.end(), deleter< _ClassInfo >() );
		}
	
		void _init_object_rtti( RflxObject* object, Klass klass, size_t baseOffset ) 
		{
			object->_outer = klass;
			_RflxClass* _class = _handle2Class( klass );
			_class->baseOffset = baseOffset;
		}	
	}

	using namespace _internal;

	RflxObject::RflxObject() : _outer( NULL )
	{
		Klass hclass = _class();
		RFLX_DASSERT( hclass );
		Rflxable* base = ( Rflxable* )this;
		size_t baseOffset = ( size_t )( (char*)base - (char*)this );
		_internal::_init_object_rtti( this, hclass, baseOffset );
		_internal::_addClassInstanceCount( hclass );
	}

	RflxObject::~RflxObject()
	{
		_internal::_releaseClassInstanceCount( _class() );
	}

	RFLX_IMPL_AUTO_REGISTER_WITH_NAME( RflxDynamicObject, "RflxDynamicObject" )

	RflxDynamicObject::RflxDynamicObject()
	{
		Klass hclass = _class();
		RFLX_DASSERT( hclass );
		RflxObject* base = ( RflxObject* )this;
		size_t baseOffset = ( size_t )( (char*)base - (char*)this );
		_internal::_init_object_rtti( this, hclass, baseOffset );
		_internal::_addClassInstanceCount( hclass );
	}

	RflxDynamicObject::~RflxDynamicObject()
	{
		_internal::_releaseClassInstanceCount( _class() );
	}

	ErrorCode initialize()
	{
		for ( int i = 0; i < vdt_max_num; ++i )
			RFLX_DASSERT( GValueDataTypeSizeNameTable[ i ].value == (ValueDataType)i );
		if ( _getContext().initialized )
			return err_ok;
		_getContext().autoRegisteringFlag = 1;
		_registerBaseObject();
		_getContext().doAutoRegister();
		_getContext().autoRegisteringFlag = 2;
		_getContext().initialized = true;
		return err_ok;
	}

	ErrorCode unInitialize()
	{
		RflxClassBank& classBank = _getContext().classBank;
		std::vector< _RflxClass* > rootClasses;
		RflxClassBank::iterator it = classBank.begin();
		while ( it != classBank.end() ) {
			if ( it->second->isRoot() )
				rootClasses.push_back( it->second );
			++it;
		}
		std::for_each( rootClasses.begin(), rootClasses.end(), _unregisterRootClass );
		classBank.clear();
		delete &_getContext();
		return err_ok;
	}

	Klass findClass( const char* name )
	{
		RFLX_DASSERT( name );
		const RflxClassBank& classBank = _getContext().classBank;
		RflxClassBank::const_iterator it = classBank.find( name );
		if ( it == classBank.end() )
			return NULL;
		return _class2Handle( it->second );
	}

	ErrorCode registerClass( const ClassInfo* classInfo, Klass* klass )
	{
		RFLX_DASSERT( klass && classInfo && classInfo->className && classInfo->className[0] != 0 );
		if ( !_getContext().initialized && _getContext().autoRegisteringFlag == 0 ) {
			if ( *klass == NULL ) {
				// mark this is already in the list which will be registered automaticlly.
				*klass = (Klass)0x01;
				_getContext().addAutoRegisterInfo( *classInfo, klass );
			}
			return err_ok;
		}
		if ( findClass( classInfo->className ) )
			return err_class_is_already_registered;
		_RflxClass* baseClass = NULL;
		if ( classInfo->baseClassName && classInfo->baseClassName[0] != 0 ) {
			Klass base = findClass( classInfo->baseClassName );
			if ( !base )
				return err_base_class_not_found;
			baseClass = _handle2Class( base );
			if ( checkClassFlag( baseClass->flags, cif_final ) ) {
				return err_wrong_inheriting;
			}
		}
		RflxClassBank& classBank = _getContext().classBank;
		std::auto_ptr< _RflxClass > newClass( new _RflxClass );
		if ( classBank.insert( std::make_pair( std::string( classInfo->className ), newClass.get() ) ).second ) {
			newClass->className = classInfo->className;
			newClass->baseClassName = _safeString( classInfo->baseClassName );
			newClass->methods = classInfo->methods;
			newClass->flags = classInfo->flags;
			if ( checkClassFlag( newClass->flags, cif_abstract ) ) { 
				newClass->methods.createInstance = NULL;	
				newClass->methods.destroyInstance = NULL;
			}
			// this needs be initialize at runtime.
			newClass->baseOffset = 0xbaadbeef;
			// properties copy and sorting by their names
			newClass->propCount = classInfo->propCount;
			if ( newClass->propCount > 0 ) {
				newClass->propDefs = new PropDef[ newClass->propCount ];
				newClass->sortedPropDefs.resize( newClass->propCount );
				for ( unsigned int i = 0; i < classInfo->propCount; ++i ) {
					newClass->propDefs[ i ] = classInfo->propDefs[ i ];
					newClass->sortedPropDefs[ i ] = newClass->propDefs + i;
				}
				std::sort( newClass->sortedPropDefs.begin(), newClass->sortedPropDefs.end(), PropDefSorter() );
			} else 
				newClass->propDefs = NULL;

			if ( baseClass )
				baseClass->children.push_back( newClass.get() );
			newClass->parent = baseClass;
			_RflxClass::initLookup( newClass.get() );
			_RflxClass::initProps( newClass.get() );
			*klass = _class2Handle( newClass.get() );
			newClass.release();
			return err_ok;
		}
		return err_failed;
	}

	ErrorCode unregisterClass( const char* name )
	{
		RFLX_DASSERT( name && name[0] );
		_RflxClass* _class = _handle2Class( findClass( name ) );
		if ( !_class )
			return err_class_not_found;
		return _unregisterClass( _class );
	}

	ErrorCode unregisterRootClass( const char* name )
	{
		RFLX_DASSERT( name && name[0] );
		_RflxClass* _class = _handle2Class( findClass( name ) );
		if ( !_class )
			return err_class_not_found;
		return _unregisterRootClass( _class );
	}

	ErrorCode getClassInfo( Klass klass, ClassInfo* classInfo )
	{
		RFLX_DASSERT( klass && classInfo );
		const _RflxClass* _class = _handle2Class( klass );
		classInfo->className = _class->className.c_str();
		classInfo->baseClassName = _class->baseClassName.c_str();
		classInfo->flags = _class->flags;
		classInfo->propCount = _class->propCount;
		classInfo->propDefs = _class->propDefs;
		classInfo->methods = _class->methods;
		return err_ok;
	}

	unsigned int getChildrenNum( Klass klass )
	{
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return ( unsigned int )_class->children.size();
	}

	Klass getBaseClass( Klass klass )
	{
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return _class2Handle( _class->parent );
	}

	const char* getClassName( Klass klass )
	{
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return _class->className.c_str();
	}

	unsigned int getClassFlags( Klass klass )
	{	
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return _class->flags;
	}

	ErrorCode createObject( const char* name, RflxObject** object )
	{
		RFLX_DASSERT( name && object );
		return createObject( findClass( name ), object );
	}

	ErrorCode createObject( Klass klass, RflxObject** object )
	{
		RFLX_DASSERT( klass && object );
		_RflxClass* _class = _handle2Class( klass );
		if ( !_class )
			return err_class_not_found;
		if ( checkClassFlag( _class->flags, cif_abstract ) )
			return err_class_is_abstract;
		if ( !_class->methods.createInstance || !_class->methods.destroyInstance )
			return err_failed;
		return _class->methods.createInstance( object );
	}

	ErrorCode destroyObject( RflxObject* object )
	{
		if ( !object ) return err_ok;
		_RflxClass* _class = _handle2Class( object->_dynamicClass() );
		if ( !_class || !_class->methods.destroyInstance )
			return err_failed;
		return _class->methods.destroyInstance( object );
	}

	ErrorCode getObjectClass( RflxObject* object, ClassInfo* info )
	{
		return getClassInfo( object->_dynamicClass(), info );
	}

	bool isInstanceOf( const RflxObject* object, Klass klass )
	{
		RFLX_DASSERT( klass && object );
		Klass thisClass = object->_dynamicClass();
		if ( thisClass == klass )
			return true;
		_RflxClass* _class = _handle2Class( thisClass );
		return _class->parentsLookup.end() != _class->parentsLookup.find( _handle2Class( klass ) );
	}

	bool isDerivesFrom( Klass klass, Klass base )
	{
		RFLX_DASSERT( klass && base );
		if ( klass == base )
			return false;
		_RflxClass* _class = _handle2Class( klass );
		return _class->parentsLookup.end() != _class->parentsLookup.find( _handle2Class( base ) );
	}

	const PropDef* getClassPropDef( Klass klass, unsigned int* count )
	{
		RFLX_DASSERT( klass );
		_RflxClass* _class = _handle2Class( klass );
		if ( count ) *count = _class->propCount;
		return _class->propDefs;
	}

	unsigned int getClassPropertyId( Klass klass, const char* name )
	{
		const PropDef* it = getClassProperty( klass, name );
		if ( it )
			return it->id;
		else
			return RFLX_INVLAID_PROPID;
	}

	const PropDef* getClassProperty( Klass klass, const char* name )
	{
		RFLX_DASSERT( klass && name && name[0] );
		const _RflxClass* _class = _handle2Class( klass );
		PropDef target;
		target.name = name;
		PropDefSorter func;
		std::vector< const PropDef* >::const_iterator it = std::lower_bound( _class->sortedPropDefs.begin(), _class->sortedPropDefs.end(), &target, func );
		if ( it != _class->sortedPropDefs.end() && !func( &target, *it ) )
			return *it;
		else
			return NULL;
	}

	const PropDef* getClassPropertyById( Klass klass, unsigned int id )
	{
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return id <= _class->propCount ? _class->propDefs + id : NULL;
	}

	const PropDef* getClassPropertyDefByPosition( const PropPos* pos )
	{	
		if ( pos ) {
			const _RflxClass* _class = _handle2Class( pos->owner );
			return _class->propDefs + pos->id;
		}
		return NULL;
	}

	static const PropPos* _getClassPropertyPosition( Klass klass, const char* name, Klass owner,
											  size_t* offset, bool recur )
	{
		static PropPos out;
		out.index = 0;
		if ( !owner || owner == klass ) {
			const PropDef* def = getClassProperty( klass, name );
			if ( def ) {
				out.id = def->id;
				out.owner = klass;
				return &out;
			}
		}
		if ( recur ) {
			const _RflxClass* _class = _handle2Class( klass );
			RFLX_DASSERT( _class );
			if ( _class->parent ) {
				*offset += _class->baseOffset;
				const PropPos* pos = _getClassPropertyPosition( _class2Handle( _class->parent ),
					name, owner, offset, recur );
				if ( pos ) {
					out.offset = *offset;
					return pos;
				}
				*offset -= _class->baseOffset;
			}
		}
		return NULL;
	}

	const PropPos* getClassPropertyPosition( Klass klass, const char* name, Klass owner, bool recur )
	{
		size_t offset = 0;
		return _getClassPropertyPosition( klass, name, owner, &offset, recur );
	}

	const PropDef* getClassPropertyEx( Klass klass, const char* fullPath )
	{
		RFLX_DASSERT( klass && fullPath && fullPath[0] );
		char path[256];
		strncpy( path, fullPath, 256 );
		char delims[] = ".";
		char* result = NULL;
		result = strtok( path, delims );
		while ( result != NULL ) {
			const PropPos* pos = getClassPropertyPosition( klass, result );
			result = pos ? strtok( NULL, delims ) : NULL;
			if ( pos ) {
				const PropDef* def = getClassPropertyDefByPosition( pos );
				if ( !result )
					return def;
				unsigned int propCount = 0;
				const PropDef* defs = NULL;
				if ( def->getDefs )
					defs = def->getDefs( &klass, &propCount, NULL );
				if ( !klass || !propCount )
					result = NULL;
			}
		}
		return NULL;
	}

	static ErrorCode _forObjectEachProperty( void* object,
		const _RflxClass* klass,
		void (*func)( const PropDef* def, void* object, void* extra ),
		void* extra )
	{
		ErrorCode ret = err_ok;
		if ( !object || !klass || !func )
			return err_failed;
		for ( unsigned int i = 0; i < klass->propCount; ++i )
			func( klass->propDefs + i, object, extra );
		if ( klass->parent ) {
			ret = _forObjectEachProperty( object, klass->parent, func, extra );
			if ( ret == err_ok )
				return ret;
		}
		return ret;
	}

	ErrorCode forObjectEachProperty( RflxObject* object,
									 void (*func)( const PropDef* def, void* object, void* extra ),
									 void* extra )
	{
		const _RflxClass* _class = _handle2Class( object->_dynamicClass() );
		if ( _class || !func )
			return err_failed;
		void* mbase = object->_mbase();
		for ( unsigned int i = 0; i < _class->propCount; ++i )
			func( _class->propDefs + i, mbase, extra );
		ErrorCode ret = err_ok;
		if ( _class->parent )
			ret = _forObjectEachProperty( mbase, _class->parent, func, extra );
		return ret;
	}

	ErrorCode forObjectEachPropertyEx( RflxObject* object,
		bool ( *filter )( Klass klass ),
		void ( *func )( const PropDef* def, void* outerObject, void* extra ),
		void* extra )
	{
		ErrorCode ret = err_ok;
		const _RflxClass* _class = _handle2Class( object->_dynamicClass() );
		if ( !_class || !func ) return err_failed;
		void* mbase = object->_mbase();
		if ( filter && filter( _class2Handle( _class ) ) ) {
			for ( unsigned int i = 0; i < _class->propCount && ret == err_ok; ++i )
				func( _class->propDefs + i, mbase, extra );
		}
		if ( _class->parent )
			ret = _forObjectEachProperty( mbase, _class->parent, func, extra );
		return ret;
	}

	const PropDef* getObjectPropertyDef( const RflxObject* object,
										 const char* name, PropPos* pos, Klass owner, bool recur )
	{
		const PropPos* p = getClassPropertyPosition( object->_dynamicClass(), name, owner, recur );
		if ( p && pos )
			*pos = *p;
		if ( p ) {
			const _RflxClass* _class = _handle2Class( p->owner );
			return _class->propDefs + p->id;
		}
		return NULL;
	}

	const PropPos* getObjectPropertyPosition( const RflxObject* object, const char* name, Klass owner, bool recur )
	{
		return getClassPropertyPosition( object->_dynamicClass(), name, owner, recur );
	}

	ErrorCode setObjectProperty( RflxObject* object, const PropPos* pos, const ValueData* value, unsigned int index )
	{
		RFLX_DASSERT( object && pos && value );
		const _RflxClass* _class = _handle2Class( pos->owner );
		const PropDef* def = _class->propDefs + pos->id;
		if ( def->set )
			return def->set( def, index, (char*)object->_mbase() + pos->offset, value );
		else return err_failed;
	}

	ErrorCode getObjectProperty( const RflxObject* object, const PropPos* pos, ValueData* value, unsigned int index )
	{
		RFLX_DASSERT( object && pos && value );
		const _RflxClass* _class = _handle2Class( object->_dynamicClass() );
		const PropDef* def = _class->propDefs + pos->id;
		if ( def->get )
			return def->get( def, index, (char*)object->_mbase() + pos->offset, value );
		else return err_failed;
	}

	ErrorCode setObjectPropertyByName( RflxObject* object, const ValueData* value, const char* name, unsigned int index, Klass owner, bool recur )
	{
		const PropPos* pos = getObjectPropertyPosition( object, name, owner, recur );
		if ( !pos ) return err_property_not_found;
		return setObjectProperty( object, pos, value, index );
	}

	ErrorCode getObjectPropertyByName( const RflxObject* object, ValueData* value, const char* name, unsigned int index, Klass owner, bool recur )
	{
		const PropPos* pos = getObjectPropertyPosition( object, name, owner, recur );
		if ( !pos ) return err_property_not_found;
		return getObjectProperty( object, pos, value, index );
	}

	ErrorCode getObjectPropertyRef( const RflxObject* object, const PropPos* pos, void** ref )
	{
		RFLX_DASSERT( object && pos && ref );
		const _RflxClass* _class = _handle2Class( object->_dynamicClass() );
		const PropDef* def = _class->propDefs + pos->id;
		*ref = (char*)object->_mbase() + def->offset + pos->offset;
		return err_ok;
	}
	
	ErrorCode getObjectPropertyRefByName( RflxObject* object, void** ref, const char* name, Klass owner, bool recur )
	{
		const PropPos* pos = getObjectPropertyPosition( object, name, owner, recur );
		if ( !pos ) return err_property_not_found;
		return getObjectPropertyRef( object, pos, ref );
	}

	const Klass* getFirstChildren( Klass klass )
	{
		const _RflxClass* _class = _handle2Class( klass );
		if ( _class->children.empty() )
			return NULL;
		return (const Klass*)&_class->children[0];
	}

	unsigned int getChildNum( Klass klass )
	{
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return (unsigned int)_class->children.size();
	}

	unsigned int getClassInstanceCount( Klass klass )
	{
		RFLX_DASSERT( klass );
		const _RflxClass* _class = _handle2Class( klass );
		return (unsigned int)_class->instanceCount;
	}

	const char* fixClassName( const char* name )
	{
		const char* p = strstr( name, " " );
		return p ? p : name;
	}

	const char* removeNamespace( const char* name )
	{
		const char* p = name;
		int len = (int)strlen( name );
		for ( int i = len - 1; i > 0; --i ) {
			if ( p[i] == ':' && p[i-1] == ':' )
				return p + i + 1;
		}
		return name;
	}

	EnumInfo::EnumInfo( const char* _name, const EnumValue* _data, unsigned int _count ) : name( _name ),
		data( _data ), count( _count )
	{
	}

	bool EnumInfo::getValue( const char* name, long* value, unsigned int* index ) const
	{
		for ( unsigned int i = 0; i < count; ++i ) {
			if ( strcmp( name, data[ i ].name ) == 0 ) {
				if ( value ) *value = data[ i ].value;
				if ( index ) *index = i;
				return true;
			}
		}
		return false;
	}

	unsigned int EnumInfo::getValueIndex( long value ) const
	{
		for ( unsigned int i = 0; i < count; ++i ) {
			if ( data[ i ].value == value )
				return i;
		}
		return (unsigned int)-1;
	}

	const EnumValue* EnumInfo::findItem( const char* name ) const
	{
		for ( unsigned int i = 0; i < count; ++i ) {
			if ( strcmp( name, data[ i ].name ) == 0 ) {
				return data + i;
			}
		}
		return NULL;
	}

	const EnumValue* EnumInfo::findItemByValue( long value ) const
	{
		for ( unsigned int i = 0; i < count; ++i ) {
			if ( value == data[ i ].value )
				return data + i;
		}
		return NULL;
	}

	const EnumValue* EnumInfo::getItemByIndex( unsigned int index ) const
	{
		if ( index >= count ) return NULL;
		return data + index;
	}

	const EnumInfo* findEnumInfo( const char* name )
	{
		if ( !name || name[ 0 ] == 0 ) return NULL;
		std::string _name( name );
		EnumBank::iterator it = _getContext().enumBank.find( _name );
		if ( _getContext().enumBank.end() != it )
			return it->second;
		else
			return NULL;
	}

	const EnumInfo*	findEnumInfoByItemName( const char* name, long* value )
	{
		if ( !name || name[ 0 ] == 0 ) return NULL;
		const char* enumName = strstr( name, "::" );
		const EnumInfo* enumInfo = NULL;
		if ( enumName ) {
			std::string _enumName( name, enumName - name );
			EnumBank::iterator it = _getContext().enumBank.find( _enumName );
			if ( _getContext().enumBank.end() != it ) {
				enumInfo = it->second;
				const EnumValue* enumValue = enumInfo->findItem( enumName + 2 );
				if ( enumValue && value )
					*value = enumValue->value;
				return enumInfo;
			}
		} else {
			EnumBank::iterator it = _getContext().enumBank.begin();
			while ( _getContext().enumBank.end() != it ) {
				enumInfo = it->second;
				const EnumValue* enumValue = enumInfo->findItem( name );
				if ( enumValue ) {
					if ( value )
						*value = enumValue->value;
					return enumInfo;
				}
				++it;
			}
		}
		return NULL;
	}

	const PropPosEx* getClassPropertyPositionEx( Klass klass, const void* object, const char* fullPath )
	{
		static PropPosEx ret;
		memset( &ret, 0, sizeof( ret ) );
		bool succeeded = false;
		RFLX_DASSERT( object && fullPath && fullPath[0] );
		char path[256];
		strncpy( path, fullPath, 256 );
		_strtrim( path );
		char delims[] = ".";
		char* result = NULL;
		const char* mbase = (const char*)object;
		Klass curKlass = klass;
		result = strtok( path, delims );
		while ( result != NULL ) {
			// parse array index
			char* cur = result;
			char* lb = NULL;
			int idx = 0;
			while ( *cur ) {
				if ( !lb && *cur == '[' )
					lb = cur;
				if ( lb && *cur == ']' ) {
					*lb = 0;
					*cur = 0;
					idx = (int)atoi( lb + 1 );
					break;
				}
				++cur;
			}
			const PropPos* pos = getClassPropertyPosition( curKlass, result );
			result = pos ? strtok( NULL, delims ) : NULL;
			if ( pos ) {
				ret = *pos;
				PropPosEx& cur = ret;
				cur.index = 0;
				cur.mbase = const_cast< char* >( mbase );
				// get current property def
				const PropDef* def = getClassPropertyDefByPosition( pos );
				if ( def->kind == vt_c_array ) {
					size_t arraySize = 0;
					operateFunctionWapper( def->op, def, NULL, op_size, &arraySize );
					cur.index = idx;
					RFLX_DASSERT( idx >= 0 && idx < (int)arraySize && "error: c array out of range!" );
				}
				else if ( def->kind > vt_container_begin && def->kind < vt_container_end ) {
					RFLX_DASSERT( idx >= 0 && "error: container index can not be negative!" );
					cur.index = idx;
				}
				if ( !result ) {
					succeeded = true;
					break;
				}
				unsigned int propCount = 0;
				const PropDef* defs = NULL;
				// if it's a dynamic object, get the outer class for next property finding.
				// else if klass will be a static class for next.
				Klass klass = NULL;
				if ( def->getDefs )
					defs = def->getDefs( &klass, &propCount, NULL );
				if ( !klass || !propCount )
					result = NULL;
				if ( def->traits_bits & ( ptf_pointer | ptf_ref ) ) {
					// pointer address
					cur.mbase = *(char**)def->get_ref( def, cur.index, cur.mbase );
					if ( def->traits_bits & ptf_rflxobject ) {
						// get the pointer object static class type
						if ( checkClassFlag( _internal::_getClassFlags( klass ), cif_polymorphic ) )
							cur.mbase += VpftSize::value;
						RflxObject* element = reinterpret_cast< RflxObject* >( cur.mbase );
						cur.mbase = (char*)element->_mbase();
						mbase = cur.mbase;
						curKlass = element->_dynamicClass();
					} else
						return NULL;
				} else {
					curKlass = klass;
					mbase = (char*)def->get_ref( def, cur.index, cur.mbase );
				}
			}
		}
		return succeeded ? &ret : NULL;
	}

	ErrorCode setObjectPropertyByNameEx( RflxObject* object, const ValueData* value, const char* fullpath, unsigned int index )
	{
		const PropPosEx* pos = getClassPropertyPositionEx( object->_dynamicClass(), object->_mbase(), fullpath );
		if ( !pos )
			return err_property_not_found;
		const PropDef* def = NULL;
		def = getClassPropertyById( pos->owner, pos->id );
		if ( !def )
			return err_property_not_found;
		return def->set( def, index + pos->index, pos->mbase, value );
	}

	ErrorCode getObjectPropertyByNameEx( const RflxObject* object, ValueData* value, const char* fullpath, unsigned int index )
	{	
		const PropPosEx* pos = getClassPropertyPositionEx( object->_dynamicClass(), object->_mbase(), fullpath );
		if ( !pos )
			return err_property_not_found;
		const PropDef* def = NULL;
		def = getClassPropertyById( pos->owner, pos->id );
		if ( !def )
			return err_property_not_found;
		return def->get( def, index + pos->index, pos->mbase, value );
	}

	ErrorCode operateFunctionWapper( pfn_operator func, const PropDef* def, void* userObject, PropOp op, ... )
	{			
		va_list va;
		va_start( va, op );
		return func( def, userObject, op, va );
	}

	ErrorCode operateObjectPropertyByName( RflxObject* object, const PropPos* pos, PropOp opType, ... )
	{
		RFLX_DASSERT( object && pos );
		const _RflxClass* _class = _handle2Class( pos->owner );
		const PropDef* def = _class->propDefs + pos->id;
		if ( def && def->op ) {
			va_list va;
			va_start( va, opType );
			return def->op( def, (char*)object->_mbase() + pos->offset, opType, va );
		}
		else return err_failed;
	}

	ErrorCode operateObjectPropertyByNameEx( const PropPosEx* pos, PropOp opType, ... )
	{
		RFLX_DASSERT( pos && pos->mbase );
		const PropDef* def = NULL;
		def = getClassPropertyById( pos->owner, pos->id );
		if ( def && def->op ) {
			va_list va;
			va_start( va, opType );
			return def->op( def, pos->mbase, opType, va );
		}
		else return err_failed;
	}

}

