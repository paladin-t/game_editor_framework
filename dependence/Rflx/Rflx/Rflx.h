#ifndef __VALUE_H__
#define __VALUE_H__

#include <string>
#include <vector>
#include <set>
#include <map>
#include <sstream>
#include <algorithm>
#include <stdarg.h>
#include <assert.h>

#ifdef _MSC_VER
	#define _CRTDBG_MAP_ALLOC
	#define _CRTDBG_MAP_ALLOC_NEW
	#include <crtdbg.h>
	#pragma warning( push )
	#pragma warning( disable : 4996 )
	#pragma warning( disable : 4201 )
	#pragma warning( disable : 4100 ) //unreferenced formal parameter
#endif

#define RFLX_TASSERT( t )			{ rflx::SAssert< (t) >::dummy_func( ); }
#define RFLX_DASSERT				assert

namespace rflx
{
	typedef wchar_t wchar;
	typedef unsigned char uchar;
	typedef unsigned short ushort;
	typedef unsigned int uint;
	typedef unsigned long ulong;
	typedef long long llong;
	typedef unsigned long long ullong;
	typedef char* pstr;
	typedef const char* pcstr;
	typedef wchar* pwstr;
	typedef const wchar* pcwstr;
	typedef void* vptr;
	typedef const void* vcptr;

	class Rflxable;
	class RflxObject;
	struct EnumInfo;
	struct EnumValue;
	struct ValueData;
	struct Message;
	struct PropDef;
	struct DVStack;
	struct ClassInfo;
	struct CustomDataHandler;
	struct _Class;
	typedef _Class* Klass;
	template< typename T > struct PackageObject;
	template< typename T > struct ExtractObject;
	template< typename T, int N > class FArray;
	template< class TClass, class TBase > class RttiBind;

	const unsigned int RFLX_INVLAID_PROPID = (unsigned int)~0;

	enum ErrorCode
	{
		err_failed = -1,
		err_ok = 0,
		err_array_out_of_range,
		err_not_implemented,
		err_class_is_being_used,
		err_class_is_already_registered,
		err_class_not_found,
		err_class_is_abstract,
		err_base_class_not_found,
		err_wrong_inheriting,
		err_invalid_params,
		err_invalid_data,
		err_property_not_found,
	};

	enum ValueDataType {
		vdt_nil = 0,
		vdt_reflexable_begin,
			vdt_base_type_begin,
				vdt_bool,
				vdt_number_begin,
					vdt_integer_number_begin,
						vdt_signed_integer_number_begin,
							vdt_char,
							vdt_short,
							vdt_int,
							vdt_long,
							vdt_llong,
						vdt_signed_integer_number_end,
						vdt_unsigned_integer_number_begin,
							vdt_wchar,
							vdt_uchar,
							vdt_ushort,
							vdt_uint,
							vdt_ulong,
							vdt_ullong,
						vdt_unsigned_integer_number_end,
					vdt_integer_number_end,
					vdt_float_number_begin,
						vdt_float,
						vdt_double,
					vdt_float_number_end,
				vdt_number_end,
			vdt_base_type_end,
			vdt_string,
			vdt_wstring,
			vdt_enum,
			vdt_custom,
			vdt_pointer,
		vdt_reflexable_end,
		vdt_max_num,
	};	

	enum ValueType
	{
		vt_scalar,
		vt_container_begin,
			vt_c_array,
			vt_array,
			vt_set,
			vt_map,
		vt_container_end,
	};

	enum ClassTypeFlag
	{
		cif_none		= 0,
		cif_abstract	= 1 << 0,
		cif_final		= 1 << 1,
		cif_polymorphic	= 1 << 2,
		cif_default		= cif_none,
	};

	enum ClassInheritType
	{
		cih_default  = 0,
		cih_public	 = cih_default,
		cih_proteced = 1 << 0,
		cih_private  = 1 << 1,
	};
	
	enum PropTraitFlag
	{
		ptf_none			= 0,
		ptf_pointer			= 1 << 0,
		ptf_pointer_const	= 1 << 1,
		ptf_ref				= 1 << 2,
		ptf_ref_const		= 1 << 3,
		ptf_const			= 1 << 4,
		ptf_serializable	= 1 << 5,
		ptf_enum			= 1 << 6,
		ptf_Volatile		= 1 << 7,
		ptf_rflxable		= 1 << 8,  ///< simplest object just has propdefs and static typeinfo
		ptf_rflxobject		= 1 << 9,  ///< object has propdefs and dynmaic typeinfo
		ptf_rflxobject_d	= 1 << 10, ///< polymorphic object has propdefs and dynmaic typeinfo
		ptf_polymorphic		= 1 << 11,
		ptf_Max				= 1 << 16,
	};

	enum PropDefFlag
	{
		pdf_none	  = 0,
		pdf_read_only = 1 << 0,
		pdf_hide	  = 1 << 1,
	};

	enum PropOp
	{
		op_size,
		op_empty,
		op_clear,
		op_resize,
		op_reserve,
		op_pop_front,
		op_pop_back,
		op_push_back,
		op_push_front,
		op_front,
		op_back,
		op_insert,
		op_getref,
		op_erase,
	};

	// property operation functions
	typedef ErrorCode		( *pfn_get )( const PropDef* def, unsigned int index, void* userObject, ValueData* val );
	typedef void*			( *pfn_get_ref )( const PropDef* def, unsigned int index, void* userObject );
	typedef ErrorCode		( *pfn_set )( const PropDef* def, unsigned int index, void* userObject, const ValueData* val );
	typedef const PropDef*	( *pfn_get_propdef )( Klass* klass, unsigned int* count, RflxObject* object );
	typedef ErrorCode		( *pfn_operator )( const PropDef* def, void* userObject, PropOp op, va_list va );
	
	// object method functions
	typedef ErrorCode		( *pfn_object_proc )( Message* msg, void* _this );
	typedef ErrorCode		( *pfn_create_instance )( RflxObject** object );
	typedef ErrorCode		( *pfn_destroy_instance )( RflxObject* object );
	typedef ErrorCode		( *pfn_message_filter )( Message* msg, void* _this );
	typedef ErrorCode		( *pfn_object_proc )( Message* msg, void* _this );

	// struct to hold a member or global property information
	struct PropDef
	{
		unsigned int flags; ///< PropDefFlag
		unsigned int id; ///< index in the original array	
		unsigned short traits_bits; ///< PropTraitFlag
		unsigned short traits_bits2; ///< PropTraitFlag2
		size_t offset; ///< memory offset in a class object
		const char* name;
		const char* description; ///< user comments
		const char* groupName;
		ValueDataType type;
		ValueDataType type2; ///< compound type such as: map with key type 
		ValueType kind; ///< scalar or array..
		const ValueData* defaultVal;
		const CustomDataHandler* customHandler; ///< for user define data type
		const CustomDataHandler* customHandler2; ///< for user define data type
		pfn_get get;
		pfn_get_ref get_ref;
		pfn_set set;
		pfn_get_propdef getDefs; ///< get perperty defs for embedded RflxObject
		pfn_operator op; ///< vary operation hanlde with command type and variables
	};

	struct EnumValue {
		const char* name;
		long value;
	};

	struct EnumInfo {
		const char* name;
		const EnumValue* data;
		unsigned int count;
		EnumInfo( const char* _name, const EnumValue* _data, unsigned int _count );
		// find a enum by a enum name
		bool getValue( const char* name, long* value = NULL, unsigned int* index = NULL ) const;
		// get index of enum declaration order by enum value.
		// return -1 if not found.
		unsigned int getValueIndex( long value ) const;
		const EnumValue* findItem( const char* name ) const;
		const EnumValue* findItemByValue( long value ) const;
		const EnumValue* getItemByIndex( unsigned int index ) const;
	};

	template< typename T, bool IsEnumType = IsEnum< T >::value >
	struct GetEnumInfo;

	struct ObjectMethods {
		pfn_object_proc	objectProc;
		pfn_create_instance	createInstance;
		pfn_destroy_instance destroyInstance;
	};

	struct ClassInfo {
		const char* className;
		const char* baseClassName;
		unsigned int flags; // ClassTypeFlag
		ObjectMethods methods;
		const PropDef* propDefs;
		unsigned int propCount;
	};

	struct PropPos {
		Klass owner;
		unsigned int id; // property index in owner class
		size_t offset; //offset to owner from outer class
		unsigned int index; // index array type object
		PropPos( ) : owner( NULL ), id( 0 ), offset( 0 ), index( 0 ) { }
	};

	struct PropPosEx : PropPos
	{
		char* mbase;
		PropPosEx( const PropPos& o ) : mbase( NULL ) {
			owner = o.owner;
			id = o.id;
			offset = o.offset;
			index = o.index;
		}
		PropPosEx() : mbase( NULL ) {}
	};

	inline unsigned int	makeClassFlag( ClassInheritType inheritType, ClassTypeFlag classType );
	inline void	setClassFlag( unsigned int& flag, ClassTypeFlag classType );
	inline void	setInheritType( unsigned int& flag, ClassInheritType inheritType );
	inline ClassInheritType getInheritType( unsigned int flag );
	inline bool	checkClassFlag( unsigned int flag, ClassTypeFlag classType );

	template< bool > struct SAssert;
	template<> struct SAssert< true > { static inline void dummy_func(){} };

	template< typename T1, typename T2 >
	struct TypeEqual { static const bool value = false; };
	template< typename T >
	struct TypeEqual< T, T > { static const bool value = true; };

	template< typename TClass >
	struct IsPtr { static const bool value = false; };
	template< typename TClass >
	struct IsPtr< TClass* > { static const bool value = true; };

	template< typename TClass >
	struct IsPtrConst { static const bool value = false; };
	template< typename TClass >
	struct IsPtrConst< TClass* const > { static const bool value = true; };

	template< typename TClass >
	struct IsConst { static const bool value = false; };
	template< typename TClass >
	struct IsConst< const TClass > { static const bool value = true; };
	
	template< typename TClass >
	struct IsRef { static const bool value = false; };
	template< typename TClass >
	struct IsRef< TClass& > { static const bool value = true; };

	template< typename TClass >
	struct IsVolatile { static const bool value = false; };
	template< typename TClass >
	struct IsVolatile< volatile TClass > { static const bool value = true; };

	template< typename TClass >
	struct IsCArray { static const bool value = false; typedef TClass type; };
	template< typename TClass >
	struct IsCArray< TClass[] > { static const bool value = true; typedef TClass type; };
	template< typename TClass, int N >
	struct IsCArray< TClass[N] > { static const bool value = true; typedef TClass type; };

	template< typename TClass >
	struct RemoveConst { typedef TClass type; };
	template< typename TClass >
	struct RemoveConst< const TClass > { typedef TClass type; };

	template< typename TClass >
	struct RemovePtr { typedef TClass type; };
	template< typename TClass >
	struct RemovePtr< TClass* > { typedef TClass type; };

	template< typename TClass >
	struct RemoveRef { typedef TClass type; };
	template< typename TClass >
	struct RemoveRef< TClass& > { typedef TClass type; };

	template< typename TClass >
	struct RemoveVolatile { typedef TClass type; };
	template< typename TClass >
	struct RemoveVolatile< volatile TClass > { typedef TClass type; };

	template< typename TClass >
	struct RemoveCV { typedef typename RemoveVolatile< typename RemoveConst< TClass >::type >::type type; };
	template< typename TClass >
	struct RemovePR { typedef typename RemovePtr< typename RemoveRef< TClass >::type >::type type; };

	template< typename TClass >
	struct RemoveOwner { typedef TClass type; };
	template< typename TOwner, typename TClass >
	struct RemoveOwner< TClass TOwner::* > { typedef TClass type; };

	template< typename TClass >
	struct IsRefConst { static const bool value = IsRef< TClass >::value && IsConst< typename RemoveConst< TClass > >::value; };

	template< bool V, typename A, typename B >
	struct If { typedef A type; };
	template< typename A, typename B >
	struct If< false, A, B > { typedef B type; };

	struct VoidType{};
	struct NullType{};
	struct HeadType{};
	struct Bool_True{ char dummy[1]; };
	struct Bool_False{ char dummy[2]; };
	template< bool B > struct BoolType { typedef Bool_False type; };
	template<> struct BoolType< true > { typedef Bool_True type; };

	template< typename T >
	class IsEnum {
		struct _IntConvert { _IntConvert( int ); };
		static Bool_False _IntConvertTestFunc( ... );
		static Bool_True  _IntConvertTestFunc( _IntConvert );
		static Bool_False _PtrTestFunc( const volatile signed char* );
		static Bool_False _PtrTestFunc( const volatile unsigned char* );
		static Bool_False _PtrTestFunc( const volatile short* );
		static Bool_False _PtrTestFunc( const volatile unsigned short* );
		static Bool_False _PtrTestFunc( const volatile int* );
		static Bool_False _PtrTestFunc( const volatile unsigned int* );
		static Bool_False _PtrTestFunc( const volatile long* );
		static Bool_False _PtrTestFunc( const volatile unsigned long* );
		static Bool_False _PtrTestFunc( const volatile long long* );
		static Bool_False _PtrTestFunc( const volatile unsigned long long* );
		static Bool_False _PtrTestFunc( const volatile double* );
		static Bool_False _PtrTestFunc( const volatile long double* );
		static Bool_False _PtrTestFunc( const volatile float* );
		static Bool_False _PtrTestFunc( const volatile bool* );
		static Bool_True  _PtrTestFunc( const volatile void* );
		static T arg;
	public:
		static const bool value = ( ( sizeof( _IntConvertTestFunc( arg ) ) == sizeof( Bool_True ) ) &&
			( sizeof( _PtrTestFunc( &arg ) ) == sizeof( Bool_True ) ) );
	};

	template< typename T >
	class IsClass {
		template< typename T1 >
		static char _Tester( int T1::* );
		template< typename T1 >
		static int _Tester( ... );
	public:
		static const bool value = sizeof( _Tester< T >( 0 ) ) == sizeof( char ) ? true : false;
	};

	template< class TDerived, class TBase >
	class IsKindOf {
	private:
		typedef typename RemoveCV< TBase >::type _TBase;
		typedef typename RemoveCV< TDerived >::type _TDerived;
		static int  _TestFunc( ... );
		static char _TestFunc( _TBase* );
	public:
		static const bool value = IsClass< _TDerived >::value &&
			IsClass< _TBase >::value && sizeof( _TestFunc( reinterpret_cast< _TDerived* >( 0 ) ) ) == sizeof( char );
	};

	template< class TDerived, class TBase >
	class IsChildOf {
	private:
		typedef typename RemoveCV< TBase >::type _TBase;
		typedef typename RemoveCV< TDerived >::type _TDerived;
		static int  _TestFunc( ... );
		static char _TestFunc( _TBase* );
	public:
		static const bool value = IsClass< _TDerived >::value &&
			IsClass< _TBase >::value && TypeEqual< _TDerived, _TBase >::value == false && sizeof( _TestFunc( reinterpret_cast< _TDerived* >( 0 ) ) ) == sizeof( char );
	};

	template< typename T, bool isClass = false >
	struct IsPolymorphicHelper { static const bool value = false; };

	template< typename T >
	struct IsPolymorphicHelper< T, true > {
		typedef typename RemoveCV< T >::type base_type;
		struct class1 : base_type{ ~class1(){} class1& operator = ( const class1& ); };
		struct class2 : base_type{ virtual ~class2(){} class2& operator = ( const class2& ); };
		static const bool value = ( sizeof( class1 ) == sizeof( class2 ) );
	};

	class VpftSize {
		struct base{ int m; };
		struct base1 : base{ virtual ~base1(){} };
	public:
		static const unsigned int value = sizeof( base1 ) - sizeof( base );
	};

	template< typename T >
	struct IsPolymorphic {
		static const bool value = IsPolymorphicHelper< T, IsClass< T >::value >::value;
		static const unsigned int vftSize = value ? VpftSize::value : 0;
	};

	template< class T >
	struct RflxObjectName {
		static const char* name( const char* _name = NULL ) {
			static char _n[64] = {0};
			if ( _n[0] == 0 && _name ) {
				const int count = sizeof(_n) / sizeof(_n[0]);
				std::char_traits< char >::copy( _n, _name, count );
			}
			return _n;
		}
	};

	namespace _internal {
		void _init_object_rtti( RflxObject* object, Klass klass, size_t baseOffset );
		unsigned int _addClassInstanceCount( Klass klass );
		unsigned int _releaseClassInstanceCount( Klass klass );
		inline unsigned int _getClassFlags( Klass klass ) { return *( unsigned int* )klass; }
	}

	// nopolymorphic and no rtti basic object
	class Rflxable {
	public:
		static const char* _name() { return RflxObjectName< Rflxable >::name( "Rflxable" ); }
		static const PropDef* _getPropDefs( unsigned int* count ){ if ( count ) *count = 0; return NULL; }
		static const Klass& _class() { static Klass c = NULL; return c; }
		static ErrorCode _createInstance( RflxObject** object ) { if ( object ) *object = NULL; return err_not_implemented; }
		static ErrorCode _destroyInstance( RflxObject* object ) { return err_not_implemented; }
		static ErrorCode _objectProc( Message* msg, void* thisObject ) { return err_ok; }
	};

	// nopolymorphic rtti basic object
	class RflxObject : public Rflxable {
		friend void _internal::_init_object_rtti( RflxObject* object, Klass klass, size_t baseOffset );
	public:
		RflxObject();
		~RflxObject();
		static const char* _name() { return RflxObjectName< RflxObject >::name( "RflxObject" ); }
		static const PropDef* _getPropDefs( unsigned int* count ){ if ( count ) *count = 0; return NULL; }
		static const Klass& _class() { static Klass c = NULL; return c; }
		static ErrorCode _createInstance( RflxObject** object ) { if ( object ) *object = NULL; return err_not_implemented; }
		static ErrorCode _destroyInstance( RflxObject* object ) { return err_not_implemented; }
		Klass _dynamicClass() const { return _outer; }
		const void* _mbase() const { return const_cast< RflxObject* >( this )->_mbase(); }
		void* _mbase() {
			if ( checkClassFlag( _internal::_getClassFlags( _outer ), cif_polymorphic ) )
				return (char*)this - VpftSize::value;
			else
				return this;
		}
	private:
		Klass	_outer;
	};

	template< class TClass, class TBase >
	class RttiBind : public TBase {
	public:
		RttiBind() {
			TClass* object = ( TClass* )this;
			TBase* baseObject = ( TBase* )this;
			size_t baseOffset = ( size_t )( (char*)baseObject - (char*)object );
			rflx::_internal::_init_object_rtti( object, TClass::_class(), baseOffset );
			rflx::_internal::_addClassInstanceCount( TClass::_class() );
		}
		~RttiBind() { rflx::_internal::_releaseClassInstanceCount( TClass::_class() ); }
	};

	namespace _internal {
		void _init_object_rtti( RflxObject* object, Klass klass, size_t baseOffset );
	}

	template< typename T, int N >
	class FArray {
	public:
		typedef T value_type;
		inline const T& operator[]( size_t i ) const { RFLX_DASSERT( i >= 0 && i < N && "FArray: out of range!" ); return array[i]; }
		inline T& operator[]( size_t i ) { RFLX_DASSERT( i >= 0 && i < N && "FArray: out of range!" ); return array[i]; }
		inline operator const T* () const { return array; }
		inline operator T* () { return array; }
	private:
		T array[N];
	};

#define RFLX_BASETYPE_TRAIT_BASE( VALUE_CLASS ) \
	static const ValueType value_class = VALUE_CLASS;\
	static const bool is_integer = type_value > vdt_integer_number_begin && type_value < vdt_integer_number_end;\
	static const bool is_float = type_value > vdt_float_number_begin && type_value < vdt_float_number_end;\
	static const bool is_signed = ( type_value > vdt_signed_integer_number_begin && type_value < vdt_signed_integer_number_end ) || is_float;\
	static const bool is_bool = type_value == vdt_bool;\
	static const bool is_const = IsConst< type >::value;\
	static const bool is_volatile = IsVolatile< type >::value;

#define RFLX_BASETYPE_TRAIT( VALUE_CLASS ) \
	RFLX_BASETYPE_TRAIT_BASE( VALUE_CLASS )\
	static const bool is_reflexable = type_value > vdt_reflexable_begin && type_value < vdt_reflexable_end;
	
	template< typename T >
	struct DataTypeTrait {
		typedef T type;
		typedef typename RemoveCV< T >::type meta_type;
	protected:
		const static bool _IsPtr = IsRef< meta_type >::value || IsPtr< meta_type >::value;
		const static bool _IsRflxable = IsKindOf< typename RemovePR< meta_type >::type, Rflxable >::value;
		const static bool _IsCArray = IsCArray< meta_type >::value;
		typedef typename IsCArray< meta_type >::type _CArrayEleType;
	public:
		const static ValueDataType type_value =
			IsEnum< meta_type >::value ? vdt_enum :
			( _IsPtr ? vdt_pointer : vdt_custom );
		RFLX_BASETYPE_TRAIT( vt_scalar );
	};

#define RFLX_IMP_DATATYPE_TRAIT( TYPE, NAME ) \
	template<> struct DataTypeTrait< TYPE > {\
		typedef TYPE type;\
		typedef TYPE meta_type;\
		static const ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};\
	template<> struct DataTypeTrait< const TYPE > {\
		typedef const TYPE type;\
		typedef TYPE meta_type;\
		static const ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};\
	template<> struct DataTypeTrait< volatile TYPE > {\
		typedef volatile TYPE type;\
		typedef TYPE meta_type;\
		static const ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};\
	template<> struct DataTypeTrait< const volatile TYPE > {\
		typedef const volatile TYPE type;\
		typedef TYPE meta_type;\
		static const ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};

	RFLX_IMP_DATATYPE_TRAIT( void, nil )
	RFLX_IMP_DATATYPE_TRAIT( bool, bool )
	RFLX_IMP_DATATYPE_TRAIT( char, char )
	RFLX_IMP_DATATYPE_TRAIT( short, short )
	RFLX_IMP_DATATYPE_TRAIT( int, int )
	RFLX_IMP_DATATYPE_TRAIT( long, long )
	RFLX_IMP_DATATYPE_TRAIT( long long, llong )
	RFLX_IMP_DATATYPE_TRAIT( wchar_t, wchar )
	RFLX_IMP_DATATYPE_TRAIT( unsigned char, uchar )
	RFLX_IMP_DATATYPE_TRAIT( unsigned short, ushort )
	RFLX_IMP_DATATYPE_TRAIT( unsigned int, uint )
	RFLX_IMP_DATATYPE_TRAIT( unsigned long, ulong )
	RFLX_IMP_DATATYPE_TRAIT( unsigned long long, ullong )
	RFLX_IMP_DATATYPE_TRAIT( float, float )
	RFLX_IMP_DATATYPE_TRAIT( double, double )
#undef RFLX_IMP_DATATYPE_TRAIT

#define RFLX_IMP_STRING_TRAIT( TYPE, NAME ) \
	template<> struct DataTypeTrait< TYPE > {\
		typedef TYPE type;\
		typedef TYPE meta_type;\
		const static ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};\
	template<> struct DataTypeTrait< const TYPE > {\
		typedef const TYPE type;\
		typedef TYPE meta_type;\
		const static ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};\
	template<> struct DataTypeTrait< volatile TYPE > {\
		typedef volatile TYPE type;\
		typedef TYPE meta_type;\
		const static ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};\
	template<> struct DataTypeTrait< const volatile TYPE > {\
		typedef const volatile TYPE type;\
		typedef TYPE meta_type;\
		const static ValueDataType type_value = vdt_##NAME;\
		RFLX_BASETYPE_TRAIT( vt_scalar );\
	};
	RFLX_IMP_STRING_TRAIT( std::string, string )
	RFLX_IMP_STRING_TRAIT( std::wstring, wstring )
#undef RFLX_IMP_STRING_TRAIT
		

	template< typename T >
	struct IsBaseType {
		typedef typename RemoveCV< T >::type type;
		const static bool value = DataTypeTrait< type >::value_class == vt_scalar && 
			( IsEnum< type >::value || IsPtr< type >::value || IsRef< type >::value || 
			( DataTypeTrait< type >::type_value > vdt_base_type_begin && DataTypeTrait< type >::type_value < vdt_base_type_end ) ||
			DataTypeTrait< type >::type_value == vdt_string ||
			DataTypeTrait< type >::type_value == vdt_wstring );
	};

	template< typename T, int N >
	struct DataTypeTrait< FArray< T, N > > {
		typedef FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< const FArray< T, N > > {
		typedef const FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< volatile FArray< T, N > > {
		typedef volatile FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< const volatile FArray< T, N > > {
		typedef const volatile FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< T[ N ] > {
		typedef FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< const T[ N ] > {
		typedef const FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< volatile T[ N ] > {
		typedef volatile FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};

	template< typename T, int N >
	struct DataTypeTrait< const volatile T[ N ] > {
		typedef const volatile FArray< T, N > type;
		typedef FArray< T, N > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		RFLX_BASETYPE_TRAIT( vt_c_array );
	};
	
#define	RFLX_IMP_CONTAINER_TRAIT( TYPE, NAME ) \
	template< typename T >\
	struct DataTypeTrait< TYPE< T > > {\
		typedef TYPE< T > type;\
		typedef TYPE< T > meta_type;\
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;\
		RFLX_BASETYPE_TRAIT( vt_##NAME );\
	};\
	template< typename T >\
	struct DataTypeTrait< const TYPE< T > > {\
		typedef const TYPE< T > type;\
		typedef TYPE< T > meta_type;\
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;\
		RFLX_BASETYPE_TRAIT( vt_##NAME );\
	};\
	template< typename T >\
	struct DataTypeTrait< volatile TYPE< T > > {\
		typedef volatile TYPE< T > type;\
		typedef TYPE< T > meta_type;\
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;\
		RFLX_BASETYPE_TRAIT( vt_##NAME );\
	};\
	template< typename T >\
	struct DataTypeTrait< const volatile TYPE< T > > {\
		typedef const volatile TYPE< T > type;\
		typedef TYPE< T > meta_type;\
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;\
		RFLX_BASETYPE_TRAIT( vt_##NAME );\
	};
	RFLX_IMP_CONTAINER_TRAIT( std::vector, array )
	RFLX_IMP_CONTAINER_TRAIT( std::set, set )
#undef RFLX_IMP_CONTAINER_TRAIT

	template< typename KEY, typename T >
	struct DataTypeTrait< std::map< KEY, T > > {
		typedef std::map< KEY, T > type;
		typedef std::map< KEY, T > meta_type;
		const static ValueDataType type_value = IsBaseType< T >::value ? DataTypeTrait< T >::type_value : vdt_custom;
		const static ValueDataType key_type_value = IsBaseType< KEY >::value ? DataTypeTrait< KEY >::type_value : vdt_custom;
		static const bool is_reflexable = key_type_value > vdt_reflexable_begin && key_type_value < vdt_reflexable_end &&
										  type_value > vdt_reflexable_begin && type_value < vdt_reflexable_end;
		RFLX_BASETYPE_TRAIT_BASE( vt_map );
	};

#define RFLX_IMPL_ENUM_BEGIN( NAME ) \
	namespace ENUM_##NAME {\
		static const rflx::EnumInfo* _InitEnumInfo( );\
	}\
	namespace rflx {\
		template<> struct GetEnumInfo< NAME, true > {\
			inline static const rflx::EnumInfo* invoke( ) { return ENUM_##NAME::_InitEnumInfo(); }\
		};\
	}\
	namespace ENUM_##NAME {\
	static const rflx::EnumInfo* _InitEnumInfo( ) { \
			const char* name = #NAME;\
			static const rflx::EnumInfo* info = NULL;\
			static rflx::EnumValue items[] = {\

#define RFLX_IMPL_ENUM_ITEM( ITEM ) { #ITEM, ITEM },

#define RFLX_IMPL_ENUM_END \
			};\
			if ( !info )\
				info = rflx::_internal::_addEnumInfo( name, items, sizeof( items ) / sizeof( items[0] ) );\
			return info;\
		}\
		static const rflx::EnumInfo* _EnumInfo = _InitEnumInfo();\
	}

	namespace _internal
	{
		struct DataTypeSizeName { size_t size; ValueDataType value; const char* name; };
		extern const DataTypeSizeName GValueDataTypeSizeNameTable[ vdt_max_num ];
		const EnumInfo* _addEnumInfo( const char* name, const EnumValue* values, unsigned int count );
	};

	struct CustomDataHandler {
		Klass			( *klass )( );
		const char*		( *name )( );
		size_t			( *size )( );
		bool			( *equal )( const void* obj1, const void* obj2 );
		void			( *copy )( void* obj1, const void* obj2 );
		void*			( *clone )( const void* obj );
		void			( *destroy )( void* obj );
		void*			( *create )( );
	};

	template< typename T, bool IsRflxObject = IsKindOf< T, RflxObject >::value || IsKindOf< T, Rflxable >::value >
	struct GetClassHelper { static inline Klass invoke() { return NULL; } };

	template< typename T >
	struct GetClassHelper< T, true > {
		static inline Klass invoke() { return T::_class(); }
	};	

	template< typename T, bool IsCustomDataType = DataTypeTrait< T >::type_value == vdt_custom >
	struct DefaultCustomDataHandler {
		static Klass		klass() { return GetClassHelper< T >::invoke(); }
		static const char*	name( ) { return RflxObjectName< T >::name(); }
		static size_t		size( ) { return sizeof( T ); }
		static bool			equal( const void* obj1, const void* obj2 ) { return memcmp( obj1, obj2, sizeof( T ) ) == 0; }
		static void*		clone( const void* obj ) { return new T( *(T*)obj ); }
		static void			copy( void* obj1, const void* obj2 ) { memcpy( obj1, obj2, sizeof( T ) ); }
		static void			destroy( void* obj ) { delete (T*)obj; }
		static void*		create( ) { return new T; }
		static const CustomDataHandler* defaultHandler( ) {
			static CustomDataHandler handler = { klass, name, size, equal, copy, clone, destroy, create };
			return &handler;
		}
	};
	template< typename T >
	struct DefaultCustomDataHandler< T, false > { static const CustomDataHandler* defaultHandler( ){ return NULL; } };

	struct ValueData {
		union {
			bool _bool;
			wchar_t _wchar;
			signed char _char;
			unsigned char _uchar;
			short _short;
			unsigned short _ushort;
			int _int;
			unsigned int _uint;
			long _long;
			unsigned long _ulong;
			long long _llong;
			unsigned long long _ullong;
			float _float;
			double _double;
			char* _string;
			wchar_t* _wstring;
			const void* _pointer;
			struct {
				long _enum;
				const EnumInfo* _enumInfo;
			};
			struct {
				void* _custom;
				CustomDataHandler* _customHandler;
			};
		};
		ValueDataType type;
		ValueData( ) : type( vdt_nil ) { }
		~ValueData( );
		void clear( );
		ValueData( const ValueData& other );
		explicit ValueData( bool value ) : type( vdt_bool ), _bool( value ) { }
		explicit ValueData( wchar_t value ) : type( vdt_wchar ), _wchar( value ) { }
		explicit ValueData( signed char value ) : type( vdt_char ), _char( value ) { }
		explicit ValueData( unsigned char value ) : type( vdt_uchar ), _uchar( value ) { }
		explicit ValueData( short value ) : type( vdt_short ), _short( value ) { }
		explicit ValueData( unsigned short value ) : type( vdt_ushort ), _ushort( value ) { }
		explicit ValueData( int value ) : type( vdt_int ), _int( value ) { }
		explicit ValueData( unsigned int value ) : type( vdt_uint ), _uint( value ) { }
		explicit ValueData( long value ) : type( vdt_long ), _long( value ) { }
		explicit ValueData( unsigned long value ) : type( vdt_ulong ), _ulong( value ) { }
		explicit ValueData( const long long& value ) : type( vdt_llong ), _llong( value ) { }
		explicit ValueData( const unsigned long long& value ) : type( vdt_ullong ), _ullong( value ) { }
		explicit ValueData( const float value ) : type( vdt_float ), _float( value ) { }
		explicit ValueData( const double& value ) : type( vdt_double ), _double( value ) { }
		explicit ValueData( const std::string& value );
		explicit ValueData( const std::wstring& value );
		explicit ValueData( const char* value );
		explicit ValueData( const wchar_t* value );
		explicit ValueData( const void* ptr );
		explicit ValueData( const void* custom, const CustomDataHandler* handler );
		ValueData& operator = ( const ValueData& other ) { return copy( other ); }
		ValueData& operator = ( bool value ) { clear(); type = vdt_bool, _bool = value; return *this; }
		ValueData& operator = ( wchar_t value ) { clear(); type = vdt_wchar, _wchar = value; return *this; }
		ValueData& operator = ( char value ) { clear(); type = vdt_char, _char = value; return *this; }
		ValueData& operator = ( unsigned char value ) { clear(); type = vdt_uchar, _uchar = value; return *this; }
		ValueData& operator = ( short value ) { clear(); type = vdt_short, _short = value; return *this; }
		ValueData& operator = ( unsigned short value ) { clear(); type = vdt_ushort, _ushort = value; return *this; }
		ValueData& operator = ( int value ) { clear(); type = vdt_int, _int = value; return *this; }
		ValueData& operator = ( unsigned int value ) { clear(); type = vdt_uint, _uint = value; return *this; }
		ValueData& operator = ( long value ) { clear(); type = vdt_long, _long = value; return *this; }
		ValueData& operator = ( unsigned long value ) { clear(); type = vdt_ulong, _ulong = value; return *this; }
		ValueData& operator = ( const long long& value ) { clear(); type = vdt_llong, _llong = value; return *this; }
		ValueData& operator = ( const unsigned long long& value ) { clear(); type = vdt_ullong, _ullong = value; return *this; }
		ValueData& operator = ( float value ) { clear(); type = vdt_float, _float = value; return *this; }
		ValueData& operator = ( const double& value ) { clear(); type = vdt_double, _double = value; return *this; }
		ValueData& operator = ( const char* value );
		ValueData& operator = ( const wchar_t* value );
		ValueData& operator = ( const std::string& value );
		ValueData& operator = ( const std::wstring& value );
		ValueData& operator = ( const void* value );
		ValueData& assign( const std::string& value );
		ValueData& assign( const std::wstring& value );
		ValueData& assign( const char* value );
		ValueData& assign( const wchar_t* value );
		ValueData& assign( const void* custom, const CustomDataHandler* handler );
		ValueData& assign( const void* value );
		ValueData& copy( const ValueData& other );
		ValueData& swap( ValueData& other );
		bool isNil( ) const { return type == vdt_nil; }
		bool isNumber( ) const { return type > vdt_number_begin && type < vdt_number_end; }
		bool isInteger( ) const { return type > vdt_integer_number_begin && type < vdt_integer_number_end; }
		bool isBoolean( ) const { return type == vdt_bool; }
		bool isString( ) const { return type == vdt_wstring || type == vdt_string; }
		bool isUserData( ) const { return type == vdt_custom; }
		bool isPointer( ) const { return type == vdt_pointer; }
		bool isEnum( ) const { return type == vdt_enum; }
		bool isFloat( ) const { return type == vdt_float || type == vdt_double; }
		bool isSigned( ) const { return isNumber() && !( type > vdt_unsigned_integer_number_begin && type < vdt_unsigned_integer_number_end ); }
		bool operator == ( const ValueData& other );
		template< typename T >
		explicit ValueData( const T& value ) : type( vdt_nil ) { PackageObject< T >::invoke( *this, value ); }
		template< typename T >
		inline ValueData& assign( const T& value ) { PackageObject< T >::invoke( *this, value ); return *this; }
		template< typename T >
		inline ValueData& operator = ( const T& value ) { PackageObject< T >::invoke( *this, value ); return *this; }
		template< typename T >
		T Extract() const { return ExtractObject<T>( *this ); }
		ValueData cast( ValueDataType targetType ) const;
		bool toString( std::string& result ) const;
		bool fromString( ValueDataType targetType, const std::string& source );
	};

	template< typename T, typename TBase = typename RemovePR< typename RemoveCV< T >::type >::type >
	struct BaseDataCastHelper {
		typedef TBase base_type;
		inline static base_type cast( const ValueData& value ) {
			const ValueDataType typeValue = DataTypeTrait< base_type >::type_value;
			RFLX_TASSERT( typeValue > vdt_base_type_begin && typeValue < vdt_base_type_end );
			RFLX_DASSERT( value.type > vdt_base_type_begin && value.type < vdt_base_type_end );
			if ( value.type != typeValue ) {
				// TODO: warning report output
			}
			switch ( value.type ) {
				case vdt_bool: return ( base_type )( value._bool ? 1 : 0 );
				case vdt_char: return ( base_type )value._char;
				case vdt_short: return ( base_type )value._short;
				case vdt_int: return ( base_type )value._int;
				case vdt_long: return ( base_type )value._long;
				case vdt_llong: return ( base_type )value._llong;
				case vdt_wchar: return ( base_type )value._wchar;
				case vdt_uchar: return ( base_type )value._uchar;
				case vdt_ushort: return ( base_type )value._ushort;
				case vdt_uint: return ( base_type )value._uint;
				case vdt_ulong: return ( base_type )value._ulong;
				case vdt_ullong: return ( base_type )value._ullong;
				case vdt_float: return ( base_type )value._float;
				case vdt_double: return ( base_type )value._double;
			}
			RFLX_DASSERT(0);
			return 0;
		}
	};
		
	template< typename T > 
	struct BaseDataCastHelper< T, bool > {
		typedef bool base_type;
		inline static bool cast( const ValueData& value ) 	{
			RFLX_DASSERT( value.type > vdt_base_type_begin && value.type < vdt_base_type_end );
			if ( value.type != vdt_bool ) {
				// TODO: warning report output
			}
			switch ( value.type ) {
				case vdt_bool: return !!value._bool;
				case vdt_char: return !!value._char;
				case vdt_short: return !!value._short;
				case vdt_int: return !!value._int;
				case vdt_long: return !!value._long;
				case vdt_llong: return !!value._llong;
				case vdt_wchar: return !!value._wchar;
				case vdt_uchar: return !!value._uchar;
				case vdt_ushort: return !!value._ushort;
				case vdt_uint: return !!value._uint;
				case vdt_ulong: return !!value._ulong;
				case vdt_ullong: return !!value._ullong;
				case vdt_float: return !!value._float;
				case vdt_double: return !!value._double;
			}
			RFLX_DASSERT(0);
			return false;
		}
	};


	template< typename T >
	inline T ExtractValue( const ValueData& value );
	template< typename T >
	inline void PackageValue( ValueData& value, const T& input );
	template<> inline std::string ExtractValue< std::string >( const ValueData& value ) { RFLX_DASSERT( value.type == vdt_string ); return value._string; }
	template<> inline std::wstring ExtractValue< std::wstring >( const ValueData& value ) { RFLX_DASSERT( value.type == vdt_wstring ); return value._wstring; }
	template<> inline const void* ExtractValue< const void* >( const ValueData& value ) { RFLX_DASSERT( value.type == vdt_pointer ); return value._pointer; }
	template<> inline void PackageValue< std::wstring >( ValueData& value, const std::wstring& input ) { value.clear(); value.assign( input ); }
	template<> inline void PackageValue< std::string >( ValueData& value, const std::string& input ) { value.clear(); value.assign( input ); }
	template<> inline void PackageValue< const char* >( ValueData& value, const pcstr& input ) { value.clear(); value.assign( input ); }
	template<> inline void PackageValue< const wchar_t* >( ValueData& value, const pcwstr& input ) { value.clear(); value.assign( input ); }
	template<> inline void PackageValue< const void* >( ValueData& value, const vcptr& input ){ value.clear(); value.type = vdt_pointer; value._pointer = input; }
	
#define FLX_DEFINE_EXTRACTVALUE( TYPE, NAME )\
	template<> inline TYPE ExtractValue< TYPE >( const ValueData& value ) { /*RFLX_DASSERT( value.type == vdt_##NAME );*/ return BaseDataCastHelper< TYPE >::cast( value ); }\
	template<> inline void PackageValue< TYPE >( ValueData& value, const TYPE& input ){ value.clear(); value.type = vdt_##NAME; value._##NAME = input; }
	
	FLX_DEFINE_EXTRACTVALUE( bool, bool )
	FLX_DEFINE_EXTRACTVALUE( char, char )
	FLX_DEFINE_EXTRACTVALUE( wchar_t, wchar )
	FLX_DEFINE_EXTRACTVALUE( unsigned char, uchar )
	FLX_DEFINE_EXTRACTVALUE( short, short )
	FLX_DEFINE_EXTRACTVALUE( unsigned short, ushort )
	FLX_DEFINE_EXTRACTVALUE( int, int )
	FLX_DEFINE_EXTRACTVALUE( unsigned int, uint )
	FLX_DEFINE_EXTRACTVALUE( long, long )
	FLX_DEFINE_EXTRACTVALUE( unsigned long, ulong )
	FLX_DEFINE_EXTRACTVALUE( long long, llong )
	FLX_DEFINE_EXTRACTVALUE( unsigned long long, ullong )
	FLX_DEFINE_EXTRACTVALUE( float, float )
	FLX_DEFINE_EXTRACTVALUE( double, double )

	template< typename T >
	struct ExtractEnum { 
		static T invoke( const ValueData& value ) { 
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_enum );
			RFLX_DASSERT( value.type == vdt_enum );
			RFLX_DASSERT( strcmp( GetEnumInfo< T >::invoke()->name, value._enumInfo->name ) == 0 );
			return (T)value._enum;
		}
	};

	template< typename T >
	struct PackageEnum {
		static void invoke( ValueData& value, const T& input ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_enum );
			value.clear();
			value.type = vdt_enum;
			value._enum = input;
			value._enumInfo = GetEnumInfo< T >::invoke();
		}
	};

	template< typename T >
	struct ExtractPointer {
		static T invoke( const ValueData& value ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_pointer );
			RFLX_DASSERT( value.type == vdt_pointer );
			return (T)value._pointer; 
		}
	};

	template< typename T >
	struct PackagePointer {
		static void invoke( ValueData& value, const T& input ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_pointer );
			value.assign( (const void*)input );
		}
	};

	template< typename T >
	struct ExtractCustom {
		static T invoke( const ValueData& value ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_custom || DataTypeTrait< T >::value_class != vt_scalar ); 
			RFLX_DASSERT( value.type == vdt_custom && value._customHandler && value._customHandler->size() == sizeof( T ) );
			return T(*(T*)value._custom);
		}
	};

	template< typename T >
	struct PackageCustom {
		static void invoke( ValueData& value, const T& input ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_custom || DataTypeTrait< T >::value_class != vt_scalar );
			value.assign( (const void*)&input, DefaultCustomDataHandler< T >::defaultHandler() );
		}
	};

	template< typename T >
	struct ExtractRflxable {
		static T invoke( const ValueData& value ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_rflxable );
			RFLX_DASSERT( value.type == vdt_rflxable ); 
			return (T)value._object; 
		}
	};

	template< typename T > 
	struct PackageRflxable {
		static void invoke( ValueData& value, const T& input ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_rflxable );
			value.assign( (const Rflxable*)input ); 
		} 
	};

	template< typename T >
	struct ExtractBaseType {
		static T invoke( const ValueData& value ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value > vdt_base_type_begin && DataTypeTrait< T >::type_value < vdt_base_type_end ||
				DataTypeTrait< T >::type_value == vdt_string ||
				DataTypeTrait< T >::type_value == vdt_wstring ); 
			return ExtractValue< T >( value );
		}
	};

	template< typename T > 
	struct PackageBaseType {
		static void invoke( ValueData& value, const T& input ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value > vdt_base_type_begin && DataTypeTrait< T >::type_value < vdt_base_type_end ||
				DataTypeTrait< T >::type_value == vdt_string ||
				DataTypeTrait< T >::type_value == vdt_wstring );
			PackageValue< T >( value, input ); 
		}
	};

	template< typename T > 
	struct ExtractRef {
		static T invoke( const ValueData& value ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_pointer );
			RFLX_DASSERT( value.type == vdt_pointer );
			return *( typename RemoveRef< T >::type* )value._pointer;
		} 
	};

	template< typename T > 
	struct PackageRef {
		static void invoke( ValueData& value, T input ) {
			RFLX_TASSERT( DataTypeTrait< T >::type_value == vdt_pointer ); 
			value.assign( (const void*)&input ); 
		}
	};
	
	template< typename T >
	struct ExtractObject {
		static T invoke( const ValueData& value ) { 
			return typename If< DataTypeTrait< T >::value_class == vt_scalar,
				typename If< IsEnum< T >::value,
					ExtractEnum< T >,
					typename If< IsPtr< T >::value,
						ExtractPointer< T >,
						typename If< IsRef< T >::value,
							ExtractRef< T >,
							typename If< DataTypeTrait< T >::type_value == vdt_custom,
								ExtractCustom< T >,
								ExtractBaseType< T >
							>::type
						>::type
					>::type >::type,
					ExtractCustom< T > // if T is not a scalar, treat it as a custom data
			>::type::invoke( value );	
		}
	};

	template< typename T >
	struct PackageObject {
		static void invoke( ValueData& value, const T& input ) {
			typename If< DataTypeTrait< T >::value_class == vt_scalar,
				typename If< IsEnum< T >::value,
					PackageEnum< T >,
					typename If< IsPtr< T >::value,
						PackagePointer< T >,
						typename If< IsRef< T >::value,
							PackageRef< T >,
							typename If< DataTypeTrait< T >::type_value == vdt_custom,
								PackageCustom< T >,
								PackageBaseType< T >
							>::type
						>::type
					>::type >::type,
					PackageCustom< T > // if T is not a scalar, treat it as a custom data
			>::type::invoke( value, input );	
		}
	};

	template< typename T, bool IsCustomDataType = DataTypeTrait< T >::type_value == vdt_custom && IsKindOf< T, Rflxable >::value >
	struct DefaultPropCustomDataInfoAccesser 
	{ 
		static const PropDef* getPropDefs( Klass* klass, unsigned int* count, RflxObject* object = NULL ) 
		{
			Klass _klass = T::_class();
			bool isRflxObject = IsKindOf< T, RflxObject >::value;
			if ( object && isRflxObject )
				_klass = object->_dynamicClass();
			if ( klass )
				*klass = _klass;
			return T::_getPropDefs( count );
		} 
	};

	template< typename T >
	struct DefaultPropCustomDataInfoAccesser< T, false > {
		static const PropDef* getPropDefs( Klass* klass, unsigned int* count, RflxObject* object = NULL ) {
			if ( klass ) 
				*klass = NULL;
			if ( count )
				*count = 0;
			return NULL;
		}
	};

	template< typename T >
	struct ValueDataTypeAccesser {
		static void* get_ref( const PropDef* def, unsigned int, void* userObject ) {
			return (char*)userObject + def->offset;
		}
		static ErrorCode get( const PropDef* def, unsigned int, void* userObject, ValueData* val ) {
			*val = ValueData( *( const T* )( (char*)userObject + def->offset ) );
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode set( const PropDef* def, unsigned int, void* userObject, const ValueData* val ) {
			char* target = (char*)userObject + def->offset;
			*(T*)target = ExtractObject< T >::invoke( *val );
			return err_ok;
		}
		static ErrorCode op( const PropDef*, void*, PropOp, va_list ) { return err_not_implemented; }
	};

	template< typename T, int N >
	struct FArray_ValueDataTypeAccesser {
		static void* get_ref( const PropDef* def, unsigned int index, void* userObject ) {
			FArray< T, N >& _array = *( FArray< T, N >* )( (char*)userObject + def->offset );
			RFLX_DASSERT( index < N );
			return &_array[ index ];
		}
		static ErrorCode get( const PropDef* def, unsigned int index, void* userObject, ValueData* val ) {
			const FArray< T, N >& _array = *( const FArray< T, N >* )( (char*)userObject + def->offset );
			*val = _array[ index ];
			RFLX_DASSERT( index < N );
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode set( const PropDef* def, unsigned int index, void* userObject, const ValueData* val ) {
			FArray< T, N >& _array = *( FArray< T, N >* )( (char*)userObject + def->offset );
			RFLX_DASSERT( index < N );
			_array[ index ] = ExtractObject< T >::invoke( *val );
			return err_ok;
		}
		static ErrorCode op( const PropDef* def, void* userObject, PropOp opType, va_list va ) 
		{
			switch ( opType ) {
			case op_size:
				{
					size_t* _size = va_arg( va, size_t* );
					if ( _size ) *_size = ( size_t )N;
					return err_ok;
				}
				break;
			case op_empty:
				{			
					bool* _empty = va_arg( va, bool* );
					if ( _empty ) *_empty = false;
					return err_ok;
				}
				break;
			case op_getref:
				{
					size_t _index = va_arg( va, size_t );
					void** _ref = va_arg( va, void** );
					if ( _ref ) {
						FArray< T, N >& _array = *( FArray< T, N >* )( (char*)userObject + def->offset );
						*_ref = &_array[ _index ];
						RFLX_DASSERT( _index < N );
					}
				}
				break;
			default:
				return err_not_implemented;
			}
			return err_ok;
		}
	};

	template< typename T >
	struct Array_ValueDataTypeAccesser {
		static void* get_ref( const PropDef* def, unsigned int index, void* userObject ) {
			std::vector< T >& _array = *( std::vector< T >* )( (char*)userObject + def->offset );
			RFLX_DASSERT( (size_t)index < _array.size() );
			return &_array[ index ];
		}
		static ErrorCode get( const PropDef* def, unsigned int index, void* userObject, ValueData* val ) {
			const std::vector< T >& _array = *( const std::vector< T >* )( (char*)userObject + def->offset );
			if ( (size_t)index >= _array.size() )
				return err_array_out_of_range;
			*val = _array[ index ];
			RFLX_DASSERT( (size_t)index < _array.size() );
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode set( const PropDef* def, unsigned int index, void* userObject, const ValueData* val ) {
			std::vector< T >& _array = *( std::vector< T >* )( (char*)userObject + def->offset );
			if ( (size_t)index >= _array.size() )
				return err_array_out_of_range;
			RFLX_DASSERT( (size_t)index < _array.size() );
			_array[ index ] = ExtractObject< T >::invoke( *val );
			return err_ok;
		}	
		static ErrorCode op( const PropDef* def, void* userObject, PropOp opType, va_list va ) 
		{
			std::vector< T >& _array = *( std::vector< T >* )( (char*)userObject + def->offset );
			switch ( opType ) {
			case op_size:
				{
					// arg1 size_t* for output
					size_t* _size = va_arg( va, size_t* );
					if ( _size ) *_size = _array.size();
				}
				break;
			case op_empty:
				{			
					// arg1 bool*
					bool* _empty = va_arg( va, bool* );
					if ( _empty ) *_empty = false;
				}
				break;
			case op_clear:
				_array.clear();
				break;
			case op_resize: 
				{
					// arg1 size_t
					size_t _size = va_arg( va, size_t );
					_array.resize( _size );
				}
				break;
			case op_reserve:
				{
					// arg1 size_t
					size_t _size = va_arg( va, size_t );
					_array.reserve( _size );
				}
				break;
			case op_pop_back:
				_array.pop_back();
				break;
			case op_push_back:
				{
					// arg1 const ValueData*
					const ValueData* value = va_arg( va, const ValueData* );
					T _element = ExtractObject< T >::invoke( *value );
					_array.push_back( _element );
				}
				break;
			case op_front:
				{
					// arg1 T* for output
					T* _element = va_arg( va, T* );
					_element = &_array.front();
				}
				break;
			case op_back:
				{
					// arg1 T* for output
					T* _element = va_arg( va, T* );
					_element = &_array.back();
				}
				break;
			case op_insert:
				{
					// arg1 : const ValueData*
					// arg2 : insert position
					const ValueData* value = va_arg( va, const ValueData* );
					T _element = ExtractObject< T >::invoke( *value );
					size_t index = va_arg( va, size_t );
					_array.insert( _array.begin() + index, _element );
				}
				break;
			case op_erase:
				{
					// arg1: size_t index
					size_t index = va_arg( va, size_t );
					_array.erase( _array.begin() + index );
				}
				break;	
			case op_getref:
				{
					size_t _index = va_arg( va, size_t );
					void** _ref = va_arg( va, void** );
					if ( _ref ) {
						std::vector< T >& _array = *( std::vector< T >* )( (char*)userObject + def->offset );
						*_ref = &_array[ _index ];
						RFLX_DASSERT( (size_t)_index < _array.size() );
					}
				}
				break;
			default:
				return err_not_implemented;
			}
			return err_ok;
		}
	};

	template< typename T >
	struct Set_ValueDataTypeAccesser {
		static void* get_ref( const PropDef* def, unsigned int index, void* userObject ) {
			std::set< T >& _set = *( std::set< T >* )( (char*)userObject + def->offset );
			RFLX_DASSERT( (size_t)index < _set.size() );
			std::set< T >::iterator it = _set.begin();
			for ( unsigned int i = 0; i < index; ++i )
				++it;
			return &*it;
		}
		static ErrorCode get( const PropDef* def, unsigned int index, void* userObject, ValueData* val ) {
			const std::set< T >& _set = *( const std::set< T >* )( (char*)userObject + def->offset );
			if ( (size_t)index >= _set.size() )
				return err_array_out_of_range;
			std::set< T >::const_iterator it = _set.begin();
			for ( unsigned int i = 0; i < index; ++i )
				++it;
			*val = *it;
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode set( const PropDef* def, unsigned int index, void* userObject, const ValueData* val ) {
			std::set< T >& _set = *( std::set< T >* )( (char*)userObject + def->offset );
			if ( (size_t)index >= _set.size() )
				return err_array_out_of_range;		
			std::set< T >::iterator it = _set.begin();
			for ( unsigned int i = 0; i < index; ++i )
				++it;
			*it = ExtractObject< T >::invoke( *val );
			return err_ok;
		}
		static ErrorCode op( const PropDef* def, void* userObject, PropOp opType, va_list va )
		{ 			
			std::set< T >& _set = *( std::set< T >* )( (char*)userObject + def->offset );
			switch ( opType ) {
			case op_size:
				{
					// arg1 size_t* for output
					size_t* _size = va_arg( va, size_t* );
					if ( _size ) *_size = _set.size();
				}
				break;
			case op_empty:
				{			
					// arg1 bool*
					bool* _empty = va_arg( va, bool* );
					if ( _empty ) *_empty = _set.empty();
				}
				break;
			case op_clear:
				_set.clear();
				break;
			case op_insert:
				{
					// arg1 : const ValueData*
					// arg2 : return bool if success
					const ValueData* value = va_arg( va, const ValueData* );
					bool* ret = va_arg( va, bool* );
					T _element = ExtractObject< T >::invoke( *value );
					bool b = _set.insert( _element ).second;
					if ( ret ) *ret = b;
				}
				break;
			case op_erase:
				{
					// arg1 : const ValueData*
					const ValueData* value = va_arg( va, const ValueData* );
					T _element = ExtractObject< T >::invoke( *value );
					std::set< T >::iterator it = _set.find( _element );
					if ( it != _set.end() )
						_set.erase( it );
				}
				break;
			default:
				return err_not_implemented;
			}
			return err_ok;
		}
	};

	template< typename Key, typename T, bool IsIntegerKey = ( DataTypeTrait< Key >::type_value > vdt_integer_number_begin ) &&
		( DataTypeTrait< Key >::type_value < vdt_integer_number_end ) ||
		DataTypeTrait< Key >::type_value == vdt_enum && sizeof( unsigned int ) >= sizeof( Key ) >
	struct Map_ValueDataTypeAccesser;

	template< typename Key, typename T >
	struct Map_ValueDataTypeAccesser< Key, T, true > {
		static void* get_ref( const PropDef* def, unsigned int index, void* userObject ) {
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			return &_map[ (Key)index ];
		}
		static ErrorCode get( const PropDef* def, unsigned int index, void* userObject, ValueData* val ) {
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			*val = _map[ (Key)index ];
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode set( const PropDef* def, unsigned int index, void* userObject, const ValueData* val ) {
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			_map[ (Key)index ] = ExtractObject< T >::invoke( *val );
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode op( const PropDef* def, void* userObject, PropOp opType, va_list va )
		{
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			switch ( opType ) {
			case op_size:
				{
					// arg1 size_t* for output
					size_t* _size = va_arg( va, size_t* );
					if ( _size ) *_size = _map.size();
				}
				break;
			case op_empty:
				{			
					// arg1 bool*
					bool* _empty = va_arg( va, bool* );
					if ( _empty ) *_empty = _map.empty();
				}
				break;
			case op_clear:
				_map.clear();
				break;
			case op_insert:
				{
					// arg1 : const ValueData* Key
					// arg2 : const ValueData* T
					// arg3 : return bool if success
					const ValueData* keyValue = va_arg( va, const ValueData* );
					Key _key = ExtractObject< Key >::invoke( *keyValue );
					const ValueData* value = va_arg( va, const ValueData* );
					T _element = ExtractObject< T >::invoke( *value );
					bool* ret = va_arg( va, bool* );
					bool b = _map.insert( std::make_pair( _key, _element ) ).second;
					if ( ret ) *ret = b;
				}
				break;
			case op_erase:
				{
					// arg1 : const ValueData* key
					const ValueData* value = va_arg( va, const ValueData* );
					Key _key = ExtractObject< Key >::invoke( *value );
					std::map< Key, T >::iterator it = _map.find( _key );
					if ( it != _map.end() )
						_map.erase( it );
				}
				break;	
			case op_getref:
				{
					const ValueData* value = va_arg( va, const ValueData* );
					Key _key = ExtractObject< Key >::invoke( *value );
					void** _ref = va_arg( va, void** );
					if ( _ref ) {
						std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
						std::map< Key, T >::iterator it = _map.find( _key );
						if ( it != _map.end() )
							*_ref = &( it->second );
						else
							*_ref = NULL;
					}
				}
				break;
			default:
				return err_not_implemented;
			}
			return err_ok;
		}
	};

	template< typename Key, typename T >
	struct Map_ValueDataTypeAccesser< Key, T, false > {
		static void* get_ref( const PropDef* def, unsigned int index, void* userObject ) {
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			RFLX_DASSERT( (size_t)index < _map.size() );
			std::map< Key, T >::iterator it = _map.begin();
			for ( unsigned int i = 0; i < index; ++i )
				++it;
			return &( it->second );
		}
		static ErrorCode get( const PropDef* def, unsigned int index, void* userObject, ValueData* val ) {
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			if ( (size_t)index >= _map.size() )
				return err_array_out_of_range;		
			std::map< Key, T >::const_iterator it = _map.begin();
			for ( unsigned int i = 0; i < index; ++i )
				++it;
			*val = it->second;
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode set( const PropDef* def, unsigned int index, void* userObject, const ValueData* val ) {
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			if ( (size_t)index >= _map.size() )
				return err_array_out_of_range;	
			std::map< Key, T >::iterator it = _map.begin();
			for ( unsigned int i = 0; i < index; ++i )
				++it;
			it->second = ExtractObject< T >::invoke( *val );
			RFLX_DASSERT( val->type == DataTypeTrait< T >::type_value );
			return err_ok;
		}
		static ErrorCode op( const PropDef* def, void* userObject, PropOp opType, va_list va )
		{
			std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
			switch ( opType ) {
			case op_size:
				{
					// arg1 size_t* for output
					size_t* _size = va_arg( va, size_t* );
					if ( _size ) *_size = _map.size();
				}
				break;
			case op_empty:
				{			
					// arg1 bool*
					bool* _empty = va_arg( va, bool* );
					if ( _empty ) *_empty = _map.empty();
				}
				break;
			case op_clear:
				_map.clear();
				break;
			case op_insert:
				{
					// arg1 : const ValueData* Key
					// arg2 : const ValueData* T
					// arg3 : return bool if success
					const ValueData* keyValue = va_arg( va, const ValueData* );
					Key _key = ExtractObject< Key >::invoke( *keyValue );
					const ValueData* value = va_arg( va, const ValueData* );
					T _element = ExtractObject< T >::invoke( *value );
					bool* ret = va_arg( va, bool* );
					bool b = _map.insert( std::make_pair( _key, _element ) ).second;
					if ( ret ) *ret = b;
				}
				break;
			case op_erase:
				{
					// arg1 : const ValueData* key
					const ValueData* value = va_arg( va, const ValueData* );
					Key _key = ExtractObject< Key >::invoke( *value );
					std::map< Key, T >::iterator it = _map.find( _key );
					if ( it != _map.end() )
						_map.erase( it );
				}
				break;	
			case op_getref:
				{
					const ValueData* value = va_arg( va, const ValueData* );
					Key _key = ExtractObject< Key >::invoke( *value );
					void** _ref = va_arg( va, void** );
					if ( _ref ) {
						std::map< Key, T >& _map = *( std::map< Key, T >* )( (char*)userObject + def->offset );
						std::map< Key, T >::iterator it = _map.find( _key );
						if ( it != _map.end() )
							*_ref = &( it->second );
						else
							*_ref = NULL;
					}
				}
				break;
			default:
				return err_not_implemented;
			}
			return err_ok;
		}
	};

	template< typename T >
	struct DataGetSetSelector {
		const static bool is_const = DataTypeTrait< T >::is_const;
		typedef T type;
		typedef T type2;
		typedef typename RemoveConst< T >::type meta_type;
		typedef typename ValueDataTypeAccesser< meta_type > setor_type;
		typedef typename ValueDataTypeAccesser< meta_type > getor_type;
	};

	template< typename T, int N >
	struct DataGetSetSelector< FArray< T, N > > {
		const static bool is_const = DataTypeTrait< T >::is_const;
		const static unsigned int array_size = N;
		typedef T type;
		typedef T type2;
		typedef typename RemoveConst< T >::type meta_type;	
		typedef typename FArray_ValueDataTypeAccesser< T, N > setor_type;
		typedef typename FArray_ValueDataTypeAccesser< T, N > getor_type;
	};

	template< typename T, int N >
	struct DataGetSetSelector< T[ N ] > : DataGetSetSelector< FArray< T, N > > {};

	template< typename TKey, typename TValue >
	struct DataGetSetSelector< std::map< TKey, TValue > > {
		const static bool is_const = DataTypeTrait< TValue >::is_const;
		typedef TValue type;
		typedef TKey type2;
		typedef typename RemoveConst< TValue >::type meta_type;
		typedef typename Map_ValueDataTypeAccesser< TKey, TValue > setor_type;
		typedef typename Map_ValueDataTypeAccesser< TKey, TValue > getor_type;
	};	
	
	template< typename T >
	struct DataGetSetSelector< std::set< T > > {
		const static bool is_const = DataTypeTrait< T >::is_const;
		typedef T type;
		typedef T type2;
		typedef typename RemoveConst< T >::type meta_type;
		typedef typename Set_ValueDataTypeAccesser< T > setor_type;
		typedef typename Set_ValueDataTypeAccesser< T > getor_type;
	};

	template< typename T >
	struct DataGetSetSelector< std::vector< T > > {
		const static bool is_const = DataTypeTrait< T >::is_const;
		typedef T type;
		typedef T type2;
		typedef typename RemoveConst< T >::type meta_type;
		typedef typename Array_ValueDataTypeAccesser< T > setor_type;
		typedef typename Array_ValueDataTypeAccesser< T > getor_type;
	};

	template< typename T >
	struct TypeTraitsBits
	{
		const static unsigned short value =
			( IsPtr< T >::value ? ptf_pointer : 0 ) |
			( IsPtrConst< T >::value ? ptf_pointer_const : 0 ) |
			( IsRef< T >::value ? ptf_ref : 0 ) |
			( IsRefConst< T >::value ? ptf_ref_const : 0 ) |
			( IsConst< T >::value ? ptf_const : 0 ) |
			( IsVolatile< T >::value ? ptf_Volatile : 0 ) |
			( IsKindOf< typename RemovePR< T >::type, Rflxable >::value ? ptf_rflxable : 0 ) |
			( IsKindOf< typename RemovePR< T >::type, RflxObject >::value ? ptf_rflxobject : 0 ) |
			( IsKindOf< typename RemovePR< T >::type, RflxDynamicObject >::value ? ptf_rflxobject_d : 0 ) |
			( IsPolymorphic< typename RemovePR< T >::type >::value ? ptf_polymorphic : 0 ) |
			( IsEnum< T >::value ? ptf_enum : 0 );
	};

	template< typename TOwner, typename TData >
	PropDef _addPropDef( TData TOwner::*, size_t _offset, unsigned int _id, const char* _name,
		const char* _userName, const char* _groupName,
		const ValueData* _default, pfn_get _get, pfn_get_ref _get_ref, pfn_set _set, pfn_get_propdef _getDef, pfn_operator _op )
	{
		typedef typename RemoveConst< TData >::type _TData;
		typedef typename DataGetSetSelector< _TData > TDataHandelType;
		typedef typename RemoveCV< TDataHandelType::type >::type TBaseDataType;
		typedef typename RemoveCV< TDataHandelType::type2 >::type TBaseDataType2;
		RFLX_TASSERT( ( DataTypeTrait< TData >::is_reflexable ) );
		if ( !_get )
			_get = typename TDataHandelType::getor_type::get;
		if ( !_get_ref )
			_get_ref = typename TDataHandelType::getor_type::get_ref;
		if ( !_set )
			_set = typename TDataHandelType::setor_type::set;
		const CustomDataHandler* _handler = typename DefaultCustomDataHandler< TBaseDataType >::defaultHandler();
		const CustomDataHandler* _handler2 = typename DefaultCustomDataHandler< TBaseDataType2 >::defaultHandler();
		if ( !_getDef )
			_getDef = typename DefaultPropCustomDataInfoAccesser< typename RemovePR< TBaseDataType >::type >::getPropDefs;
		if ( !_op )
			_op = typename TDataHandelType::setor_type::op;
		const unsigned short traits_bits = TypeTraitsBits< typename TDataHandelType::type >::value;
		const unsigned short traits_bits2 = TypeTraitsBits< typename TDataHandelType::type2 >::value;
		PropDef def = { pdf_none | DataTypeTrait< TData >::is_const ? pdf_read_only : 0,
			_id, traits_bits, traits_bits2, _offset, _name, _userName, _groupName,
			DataTypeTrait< typename TDataHandelType::type >::type_value,
			DataTypeTrait< typename TDataHandelType::type2 >::type_value,
			DataTypeTrait< TData >::value_class,
			_default, _handler, _handler2, _get, _get_ref, _set, _getDef, _op };
		return def;
	}

	template< class T, class TBase, bool Enable = false, bool IsRflxObject = IsKindOf< TBase, RflxObject >::value >
	struct RttiInitializerHelper{};

	template< class T, class TBase >
	struct RttiInitializerHelper< T, TBase, true, false > {
		RttiInitializerHelper() { rflx::_internal::_addClassInstanceCount( T::_class() ); }
		~RttiInitializerHelper() { rflx::_internal::_releaseClassInstanceCount( T::_class() ); }
	};

	template< class T, class TBase >
	struct RttiInitializerHelper< T, TBase, true, true > {
		RttiInitializerHelper() {
			T* object = reinterpret_cast< T* >( (char*)this - offsetof( T, __RttiInitializer__ ) );
			TBase* baseObject = ( TBase* )object;
			size_t baseOffset = ( size_t )( (char*)baseObject - (char*)object );
			rflx::_internal::_init_object_rtti( object, T::_class(), baseOffset );
			rflx::_internal::_addClassInstanceCount( T::_class() );
		}
		~RttiInitializerHelper() { rflx::_internal::_releaseClassInstanceCount( T::_class() ); }
	};

	template< class TDst, class TSrc, bool IsValid = IsKindOf< TDst, RflxObject >::value && IsKindOf< TSrc, RflxObject >::value >
	struct DynamicCastHelper {
		static inline TDst* invoke( TSrc* src ) {
			if ( src && isInstanceOf( src, TDst::_class() ) ) {
				bool b1 = checkClassFlag( _internal::_getClassFlags( src->_dynamicClass() ), cif_polymorphic );
				bool b2 = checkClassFlag( _internal::_getClassFlags( TDst::_class() ), cif_polymorphic );
				if ( b1 == b2 )
					return reinterpret_cast< TDst* >( src->_mbase() );
				else {
					if ( b1 )
						return reinterpret_cast< TDst* >( (char*)src->_mbase() + VpftSize::value );
					else
						return reinterpret_cast< TDst* >( (char*)src->_mbase() - VpftSize::value );
				}
			} else
				return NULL;
		}
	};

	template< class TDst, class TSrc > struct DynamicCastHelper< TDst, TSrc, false >{};

#define RFLX_IMPL_COMMON( CLASS ) \
	typedef CLASS this_type;\
	static const rflx::Klass& _class(){ static rflx::Klass _staticClass = NULL; return _staticClass; }\
	static const char* _name() { return rflx::RflxObjectName< CLASS >::name(); }\
	static rflx::ErrorCode _unregisterClass() { return rflx::unregisterClass( CLASS::_name() ); }\
	
#define RFLX_IMPL_FACTORY_FUNC( CLASS )\
	static rflx::ErrorCode _createInstance( rflx::RflxObject** object )\
	{\
		RFLX_DASSERT( object );\
		CLASS* p = NULL;\
		try {\
			p = new CLASS();\
		} catch( ... ){ RFLX_DASSERT( 0 && "Rflx::_createInstance failed!" ); }\
		if ( p ) {\
			*object = p;\
			return rflx::err_ok;\
		} else return rflx::err_failed;\
	}\
	static rflx::ErrorCode _destroyInstance( rflx::RflxObject* object )\
	{\
		if ( !object ) return rflx::err_ok;\
		delete (CLASS*)object;\
		return rflx::err_ok;\
	}
	
#define RFLX_IMPL_NULL_FACTORY_FUNC \
	static rflx::ErrorCode _createInstance( rflx::RflxObject** ){ return rflx::err_not_implemented; }\
	static rflx::ErrorCode _destroyInstance( rflx::RflxObject* ){ return rflx::err_not_implemented; }

#define RFLX_IMPL_AUTO_REGISTER( CLASS ) \
	namespace AUTO_REGISTER_##CLASS { const static rflx::ErrorCode __auto_register_result = rflx::RflxObjectName< CLASS >::name() ? CLASS::_registerClass() : rflx::err_ok; }

#define RFLX_IMPL_AUTO_REGISTER_WITH_NAME( CLASS, NAME ) \
	namespace AUTO_REGISTER_##CLASS { const static rflx::ErrorCode __auto_register_result = rflx::RflxObjectName< CLASS >::name( NAME ) ? CLASS::_registerClass() : rflx::err_ok; }

#define RFLX_IMPL_AUTO_REGISTER_WITH_NAMEID( CLASS ) \
	namespace AUTO_REGISTER_##CLASS { const static rflx::ErrorCode __auto_register_result = rflx::RflxObjectName< CLASS >::name( typeid( NAME ).name() ) ? CLASS::_registerClass() : rflx::err_ok; }

// a member object use to do the rtti initializing if current class is not derived from rflx::RttiBind
#define RFLX_IMPL_DYAMIC_INITIALIZER RFLX_IMPL_INNER_DYAMIC_INITIALIZER
#define RFLX_IMPL_INNER_DYAMIC_INITIALIZER( CLASS, BASECLASS ) \
	rflx::RttiInitializerHelper< CLASS, BASECLASS, false == rflx::IsChildOf< CLASS, rflx::RttiBind< CLASS, BASECLASS > >::value > __RttiInitializer__;

#define RFLX_OBJECT_DINIT {\
		bool enable = !rflx::IsChildOf< this_type, rflx::RttiBind< this_type, base_type > >::value;\
		if ( enable ) {\
			base_type* baseObject = ( base_type* )this;\
			size_t baseOffset = ( size_t )( (char*)baseObject - (char*)this );\
			rflx::_internal::_init_object_rtti( this, this_type::_class(), baseOffset );\
			rflx::_internal::_addClassInstanceCount( this_type::_class() );\
		}\
	}

#define RFLX_OBJECT_DUNINIT {\
		bool enable = !rflx::IsChildOf< this_type, rflx::RttiBind< this_type, base_type > >::value;\
		if ( enable )\
			rflx::_internal::_releaseClassInstanceCount( this_type::_class() );\
	}

#define RFLX_IMPL_REGISTER_CLASS( CLASS ) \
	static rflx::ErrorCode _registerClass()\
	{\
		RFLX_TASSERT( ( rflx::IsKindOf< this_type, base_type >::value ) );\
		rflx::RflxObjectName< CLASS >::name( #CLASS );\
		rflx::Klass& klass = const_cast< rflx::Klass& >( CLASS::_class() );\
		rflx::ClassInfo cls;\
		memset( &cls, 0, sizeof( cls ) );\
		cls.className = _name();\
		cls.baseClassName = base_type::_name();\
		rflx::setClassFlag( cls.flags, rflx::IsPolymorphic< this_type >::value ? rflx::cif_polymorphic : rflx::cif_none );\
		cls.methods.createInstance = _createInstance == base_type::_createInstance ? NULL : _createInstance;\
		cls.methods.destroyInstance = _destroyInstance == base_type::_destroyInstance ? NULL : _destroyInstance;\
		cls.methods.objectProc = _objectProc == base_type::_objectProc ? NULL : _objectProc;\
		cls.propCount = 0;\
		cls.propDefs = _getPropDefs == base_type::_getPropDefs ? NULL : _getPropDefs( &cls.propCount );\
		return rflx::registerClass( &cls, &klass );\
	}

//----------------------------------------------------------------------------------------------------------------
// simple class declaration
//----------------------------------------------------------------------------------------------------------------
#define RFLX_IMPL_SIMPLE_BASE_CLASS( CLASS ) \
	protected: RFLX_IMPL_NULL_FACTORY_FUNC; public:\
	RFLX_IMPL_COMMON( CLASS )\
	typedef rflx::Rflxable base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )

#define RFLX_IMPL_SIMPLE_CLASS( CLASS, BASECLASS ) \
	protected: RFLX_IMPL_NULL_FACTORY_FUNC; public:\
	RFLX_IMPL_COMMON( CLASS )\
	typedef BASECLASS base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )

#define RFLX_IMPL_ABSTRACT_SIMPLE_BASE_CLASS( CLASS ) \
	protected: RFLX_IMPL_NULL_FACTORY_FUNC; public:\
	RFLX_IMPL_COMMON( CLASS )\
	typedef rflx::Rflxable base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )

#define RFLX_IMPL_ABSTRACT_SIMPLE_CLASS( CLASS, BASECLASS ) \
	protected: RFLX_IMPL_NULL_FACTORY_FUNC; public:\
	RFLX_IMPL_COMMON( CLASS )\
	typedef BASECLASS base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )

//----------------------------------------------------------------------------------------------------------------
// general class declaration
//----------------------------------------------------------------------------------------------------------------
#define RFLX_IMPL_ABSTRACT_BASE_CLASS( CLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef rflx::RflxObject base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )\
	RFLX_IMPL_INNER_DYAMIC_INITIALIZER( CLASS, base_type )

#define RFLX_IMPL_BASE_CLASS( CLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef rflx::RflxObject base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )\
	RFLX_IMPL_FACTORY_FUNC( CLASS )\
	RFLX_IMPL_INNER_DYAMIC_INITIALIZER( CLASS, base_type )

#define RFLX_IMPL_CLASS( CLASS, BASECLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef BASECLASS base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )\
	RFLX_IMPL_FACTORY_FUNC( CLASS )\
	RFLX_IMPL_INNER_DYAMIC_INITIALIZER( CLASS, base_type )

#define RFLX_IMPL_ABSTRACT_CLASS( CLASS, BASECLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef BASECLASS base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )\
	RFLX_IMPL_INNER_DYAMIC_INITIALIZER( CLASS, base_type )
//----------------------------------------------------------------------------------------------------------------
// general class declaration
//----------------------------------------------------------------------------------------------------------------
#define RFLX_IMPL_MANUAL_ABSTRACT_BASE_CLASS( CLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef rflx::RflxObject base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )

#define RFLX_IMPL_MANUAL_BASE_CLASS( CLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef rflx::RflxObject base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )\
	RFLX_IMPL_FACTORY_FUNC( CLASS )

#define RFLX_IMPL_MANUAL_CLASS( CLASS, BASECLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef BASECLASS base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )\
	RFLX_IMPL_FACTORY_FUNC( CLASS )

#define RFLX_IMPL_MANUAL_ABSTRACT_CLASS( CLASS, BASECLASS ) \
	RFLX_IMPL_COMMON( CLASS )\
	typedef BASECLASS base_type;\
	RFLX_IMPL_REGISTER_CLASS( CLASS )
//----------------------------------------------------------------------------------------------------------------
// property reflection macros
//----------------------------------------------------------------------------------------------------------------	
#define RFLX_BEGIN_PROPERTY_MAP\
	static const rflx::PropDef* _getPropDefs( unsigned int* count )\
    { \
        typedef this_type _currClass; \
		unsigned int _start = 0;\
		static rflx::PropDef _defs[] = {
		
#define RFLX_IMPL_PROPERTY_DEF( PROP, PROPNAME, DESC, GROUPNAME, DEFAULT, GET, GETREF, SET, GETDEF, OPERATE ) \
	rflx::_addPropDef( &_currClass::PROP, offsetof( _currClass, PROP ),  _start++, PROPNAME, DESC, GROUPNAME, DEFAULT, GET, GETREF, SET, GETDEF, OPERATE ),

#define RFLX_END_PROPERTY_MAP \
		}; *count = sizeof(_defs)/sizeof(_defs[0]); \
        return _defs;\
    }

#define RFLX_IMPLEMENT_EMPTY_PROPERTY_MAP \
    static const PropDef *_getPropDefs( unsigned int* count ){\
        *count = 0;\
        return NULL;\
    }

#define RFLX_IMPL_PROPERTY( PROP ) RFLX_IMPL_PROPERTY_DEF( PROP, #PROP, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL )
#define RFLX_IMPL_PROPERTY_NAME( PROP, PROPNAME ) RFLX_IMPL_PROPERTY_DEF( PROP, PROPNAME, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL )
#define RFLX_IMPL_PROPERTY_NAME_DESC( PROP, PROPNAME, DESCRIPTION ) RFLX_IMPL_PROPERTY_DEF( PROP, PROPNAME, DESCRIPTION, NULL, NULL, NULL, NULL, NULL, NULL, NULL )
//----------------------------------------------------------------------------------------------------------------	
	namespace DValueStack
	{
		DVStack*			create( size_t maxSize = 0 );
		void				destroy( DVStack* stack );
		DVStack&			getGlobal();
		size_t				maxSize( const DVStack& stack );
		size_t				size( const DVStack& stack );
		ValueData&			getAt( DVStack& stack, int index );
		const ValueData&	getAt( const DVStack& stack, int index );
		void				pop( DVStack& stack );
		void				push( DVStack& stack, const ValueData& value );
		ValueData			popData( DVStack& stack );

		template< typename T >
		void pushValue( const DVStack& stack, const T& value ) {
			ValueData dv;
			PackageObject< T >::invoke( dv, value );
			push( stack, dv );
		}

		template< typename T >
		void popValue( const DVStack& stack, T& value ) { 
			value = ExtractObject< T >::invoke( popData( stack ) );
		}

		template< typename T >
		T getValue( const DVStack& stack, int index ) {
			const ValueData& dv = getAt( stack, index );
			value = ExtractObject< T >::invoke( getAt( stack, index ) );
		}
	}
	inline size_t			getValueDataTypeSize( ValueDataType type ) { return _internal::GValueDataTypeSizeNameTable[ type ].size; }
	inline const char*		getValueDataTypeName( ValueDataType type ) { return _internal::GValueDataTypeSizeNameTable[ type ].name; }
	inline unsigned int		makeClassFlag( ClassInheritType inheritType, ClassTypeFlag classType ){ return ( ( (unsigned int)inheritType & 0xff ) << 8 ) | ( (unsigned int)classType & 0xff );}
	inline void				setClassFlag( unsigned int& flag, ClassTypeFlag classType ) { flag |= ( (unsigned int)classType & 0xff ); }
	inline void				setInheritType( unsigned int& flag, ClassInheritType inheritType ) { flag &= 0xffff00ff; flag |= ( (unsigned int)inheritType & 0xff ) << 8; }
	inline ClassInheritType getInheritType( unsigned int flag ) { return (ClassInheritType)( ( flag & 0x0000ff00 ) >> 8 ); }
	inline bool				checkClassFlag( unsigned int flag, ClassTypeFlag classType ) { return !!( flag & classType ); }

	ErrorCode			registerClass( const ClassInfo* classInfo, Klass* klass = NULL );
	ErrorCode			unregisterClass( const char* name );

	class RflxDynamicObject : public RflxObject {
	public:
		RFLX_IMPL_BASE_CLASS( RflxDynamicObject )
		RflxDynamicObject();
		virtual ~RflxDynamicObject();
	};

	template< class TDst, class TSrc >
	static inline TDst* dynamicCast( TSrc* src ) { return DynamicCastHelper< TDst, TSrc >::invoke( src ); }


	ErrorCode			initialize( );
	ErrorCode			unInitialize( );
	Klass				findClass( const char* name );
	ErrorCode			unregisterRootClass( const char* name );
	ErrorCode			getClassInfo( Klass klass, ClassInfo* classInfo );
	ErrorCode			createObject( const char* name, RflxObject** object );
	ErrorCode			createObject( Klass klass, RflxObject** object );
	ErrorCode			destroyObject( RflxObject* object );
	ErrorCode			getObjectClass( RflxObject* object, ClassInfo* info );
	bool				isInstanceOf( const RflxObject* object, Klass klass );
	bool				isDerivesFrom( Klass klass, Klass base );
	unsigned int		getChildrenNum( Klass klass );
	Klass				getBaseClass( Klass klass );
	const char*			getClassName( Klass klass );
	unsigned int		getClassFlags( Klass klass );
	const PropDef*		getClassPropDef( Klass klass, unsigned int* count );
	unsigned int		getClassPropertyId( Klass klass, const char* name );
	const PropDef*		getClassProperty( Klass klass, const char* name );
	const PropDef*		getClassPropertyById( Klass klass, unsigned int id );
	const PropDef*		getClassPropertyDefByPosition( const PropPos* pos );
	const PropDef*		getClassPropertyEx( Klass klass, const char* fullPath );
	const PropPos*		getClassPropertyPosition( Klass klass, const char* name, Klass owner = NULL, bool recur = true );
	const PropPosEx*	getClassPropertyPositionEx( Klass klass, const void* object, const char* fullPath );
	ErrorCode			forObjectEachProperty( RflxObject* object, void (*func)( const PropDef* def, void* object, void* extra ), void* extra = NULL );
	ErrorCode			forObjectEachPropertyEx( RflxObject* object, void ( *filter )( Klass klass ), void ( *func )( const PropDef* def, void* outerObject, void* extra ), void* extra = NULL );
	const PropDef*		getObjectPropertyDef( const RflxObject* object, const char* name, PropPos* pos, Klass owner = NULL, bool recur = true );
	const PropPos*		getObjectPropertyPosition( const RflxObject* object, const char* name, Klass owner = NULL, bool recur = true );
	ErrorCode			setObjectProperty( RflxObject* object, const PropPos* pos, const ValueData* value, unsigned int index = 0 );
	ErrorCode			getObjectProperty( const RflxObject* object, const PropPos* pos, ValueData* value, unsigned int index = 0 );
	ErrorCode			setObjectPropertyByName( RflxObject* object, const ValueData* value, const char* name, unsigned int index = 0, Klass owner = NULL, bool recur = true );
	ErrorCode			getObjectPropertyByName( const RflxObject* object, ValueData* value, const char* name, unsigned int index = 0, Klass owner = NULL, bool recur = true );
	ErrorCode			setObjectPropertyByNameEx( RflxObject* object, const ValueData* value, const char* fullpath, unsigned int index = 0 );
	ErrorCode			getObjectPropertyByNameEx( const RflxObject* object, ValueData* value, const char* fullpath, unsigned int index = 0 );
	ErrorCode			getObjectPropertyRef( const RflxObject* object, const PropPos* pos, void** ref );
	ErrorCode			getObjectPropertyRefByName( RflxObject* object, void** ref, const char* name, Klass owner = NULL, bool recur = true );
	ErrorCode			operateFunctionWapper( pfn_operator func, const PropDef* def, void* userObject, PropOp op, ... );
	ErrorCode			operateObjectPropertyByName( RflxObject* object, const PropPos* pos, PropOp opType, ... );
	ErrorCode			operateObjectPropertyByNameEx( const PropPosEx* pos, PropOp opType, ... );
	const Klass*		getFirstChildren( Klass klass );
	unsigned int		getChildNum( Klass klass );
	unsigned int		getClassInstanceCount( Klass klass );
	const char*			fixClassName( const char* name );
	const char*			removeNamespace( const char* name );
	const EnumInfo*		findEnumInfo( const char* name );
	const EnumInfo*		findEnumInfoByItemName( const char* name, long* value );
}

#ifdef _MSC_VER
	#pragma warning( pop )
#endif

#endif
