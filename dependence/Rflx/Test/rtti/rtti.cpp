#include "../../Rflx/Rflx.h"
#include <iostream>
using namespace rflx;
enum ELanguage
{
	EL_Chinese,
	EL_English,
};

RFLX_IMPL_ENUM_BEGIN( ELanguage )
	RFLX_IMPL_ENUM_ITEM( EL_Chinese )
	RFLX_IMPL_ENUM_ITEM( EL_English )
RFLX_IMPL_ENUM_END

struct SimpleBaseClass : public Rflxable
{
	RFLX_IMPL_SIMPLE_BASE_CLASS( SimpleBaseClass )
	int x;
	int y;
};

struct SimpleClass1 : public SimpleBaseClass
{
	RFLX_IMPL_SIMPLE_CLASS( SimpleClass1, SimpleBaseClass )
	int z;
};

struct SimpleClass2 : public SimpleClass1
{
	RFLX_IMPL_SIMPLE_CLASS( SimpleClass2, SimpleClass1 )
	int w;
	virtual ~SimpleClass2(){};
};

RFLX_IMPL_AUTO_REGISTER( SimpleBaseClass )
RFLX_IMPL_AUTO_REGISTER( SimpleClass1 )
RFLX_IMPL_AUTO_REGISTER( SimpleClass2 )

class Class1 : public RflxObject
{
public:
	RFLX_IMPL_BASE_CLASS( Class1 )
	int m_int;
};

class Class2 : public Class1
{
public:
	virtual void func(){}
	RFLX_IMPL_CLASS( Class2, Class1 )
	short m_short;
};

class Class3 : public RttiBind< Class3, Class2 >
{
public:
	RFLX_IMPL_MANUAL_CLASS( Class3, Class2 )
	Class3()
	{
		// if base class is RttiBind, this macro will be disabled automaticlly
		RFLX_OBJECT_DINIT
	}
	~Class3()
	{
		// if base class is RttiBind, this macro will be disabled automaticlly
		RFLX_OBJECT_DUNINIT
	}
	int* m_pointer;
	std::string m_string;
	std::string m_sArray[10];
	std::vector< std::string > m_Array;	
	std::set< std::string > m_Set;
	std::map< std::string, SimpleBaseClass > m_Map;

	RFLX_BEGIN_PROPERTY_MAP
		RFLX_IMPL_PROPERTY( m_pointer )
		RFLX_IMPL_PROPERTY( m_string )
		RFLX_IMPL_PROPERTY( m_sArray )
		RFLX_IMPL_PROPERTY( m_Array )
		RFLX_IMPL_PROPERTY( m_Set )
		RFLX_IMPL_PROPERTY( m_Map )
	RFLX_END_PROPERTY_MAP
};

RFLX_IMPL_AUTO_REGISTER( Class1 )
RFLX_IMPL_AUTO_REGISTER_WITH_NAME( Class2, "_Class2" )
RFLX_IMPL_AUTO_REGISTER( Class3 )

int main()
{
	RFLX_TASSERT( IsEnum< ELanguage >::value );
	const rflx::EnumInfo* ei = GetEnumInfo< ELanguage >::invoke();
	std::cout << "enum name : " << ei->name << std::endl;
	RFLX_TASSERT( !IsEnum< void* >::value );
	RFLX_TASSERT( IsPtr< void* >::value );
	RFLX_TASSERT( DataTypeTrait< void >::type_value == vdt_nil );
	RFLX_TASSERT( DataTypeTrait< void* >::type_value == vdt_pointer );
	initialize();
	std::cout << "base class name list:" << std::endl;
	std::cout << getClassName( Rflxable::_class() ) << std::endl;
	std::cout << getClassName( RflxObject::_class() ) << std::endl;
	std::cout << getClassName( RflxDynamicObject::_class() ) << std::endl;
	std::cout << "end--------------------" << std::endl;
	{
		SimpleBaseClass sbase;
		SimpleClass1 s1, s11;
		SimpleClass2 s2;
		// simple class rtti check
		RFLX_DASSERT( isDerivesFrom( SimpleBaseClass::_class(), Rflxable::_class() ) );
		RFLX_DASSERT( isDerivesFrom( SimpleClass1::_class(), SimpleBaseClass::_class() ) );
		RFLX_DASSERT( isDerivesFrom( SimpleClass2::_class(), SimpleClass1::_class() ) );
		RFLX_DASSERT( !isDerivesFrom( SimpleBaseClass::_class(), RflxObject::_class() ) );
		RFLX_DASSERT( !isDerivesFrom( SimpleClass1::_class(), SimpleClass2::_class() ) );
		RFLX_DASSERT( !isDerivesFrom( Rflxable::_class(), SimpleClass1::_class() ) );
		// simple class will not do the instance count tracking.
		RFLX_DASSERT( getClassInstanceCount( SimpleBaseClass::_class() ) == 0 );
		RFLX_DASSERT( getClassInstanceCount( SimpleClass1::_class() ) == 0 );
		RFLX_DASSERT( getClassInstanceCount( SimpleClass2::_class() ) == 0 );
		RFLX_DASSERT( getClassInstanceCount( Rflxable::_class() ) == 0 );
		std::cout << "simple class name list:" << std::endl;
		std::cout << getClassName( SimpleBaseClass::_class() ) << std::endl;
		std::cout << getClassName( SimpleClass1::_class() ) << std::endl;
		std::cout << getClassName( SimpleClass2::_class() ) << std::endl;
		std::cout << "end--------------------" << std::endl;
	}
	// dynamic create test
	{
		{
			Class1 c1;
			c1;
			RFLX_DASSERT( getClassInstanceCount( Class1::_class() ) == 1 );
		}
		Class2 c2;
		Class3 c3;
		c2;
		c3;
		RFLX_DASSERT( getClassInstanceCount( Class1::_class() ) == 2 );
		RFLX_DASSERT( getClassInstanceCount( Class2::_class() ) == 2 );
		RFLX_DASSERT( getClassInstanceCount( Class3::_class() ) == 1 );
		
		RflxObject* p1 = NULL;
		// create a default named class
		createObject( "Class1", &p1 );
		RFLX_DASSERT( p1 );
		RFLX_DASSERT( p1->_dynamicClass() == Class1::_class() );
		
		RflxObject* p2 = NULL;
		// create a user named class here
		createObject( "_Class2", &p2 );
		RFLX_DASSERT( p2 );
		RFLX_DASSERT( p2->_dynamicClass() == Class2::_class() );
		
		// you could access the class flags member to know if it is a polymorphic object
		RFLX_DASSERT( !( getClassFlags( Class1::_class() ) & cif_polymorphic ) );
		RFLX_DASSERT( getClassFlags( Class2::_class() ) & cif_polymorphic );
		
		// error cast
		RflxObject* p11 = dynamicCast< Class3 >( p2 );
		RFLX_DASSERT( !p11 );
		p11 = dynamicCast< Class1 >( p2 );
		RFLX_DASSERT( p11 );

		Class2* p22 = dynamicCast< Class2 >( p11 );
		RFLX_DASSERT( p22 );
		// you can't do this because Rflxable is NOT a dynamic rtti object
		// Rflxable* p111 = dynamicCast< Rflxable >( p11 );
		
		RFLX_DASSERT( isInstanceOf( p1, Rflxable::_class() ) );
		RFLX_DASSERT( isInstanceOf( p1, RflxObject::_class() ) );
		RFLX_DASSERT( !isInstanceOf( p1, Class2::_class() ) );

		RFLX_DASSERT( isInstanceOf( p2, Rflxable::_class() ) );
		RFLX_DASSERT( isInstanceOf( p2, RflxObject::_class() ) );
		RFLX_DASSERT( isInstanceOf( p2, Class1::_class() ) );
		RFLX_DASSERT( isInstanceOf( p2, Class2::_class() ) );

		ErrorCode ret = err_ok;
		ret = destroyObject( p1 );
		RFLX_DASSERT( ret == err_ok );
		ret = destroyObject( p2 );
		RFLX_DASSERT(  ret == err_ok );
	}
	{
		struct Tiny : Rflxable
		{
			int i1;
			int i2;
			RFLX_IMPL_SIMPLE_BASE_CLASS( Tiny )
			RFLX_BEGIN_PROPERTY_MAP
				RFLX_IMPL_PROPERTY( i1 )
				RFLX_IMPL_PROPERTY( i2 )
			RFLX_END_PROPERTY_MAP
		};
		Tiny::_registerClass();
		std::cout << "dynamic register class :" <<  getClassName( Tiny::_class() ) << std::endl;
		Tiny::_unregisterClass();
		// this is important: we could use memset
		RFLX_TASSERT( sizeof( Tiny ) == sizeof( int ) + sizeof( int ) );
		Tiny tiny;
		memset( &tiny, 0, sizeof( Tiny ) );
	}


	unInitialize();
	return 0;
}

