#include "stdafx.h"

using namespace std;
using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace simply
{
    TEST_CLASS(type_traits_test)
    {
        wstring output;
        utility::temporary<function<void(const wstring&)>> fail_stub {
            assert::implementation::fail,
            [&](const wstring& message) { output = message; }
        };

        class abstract_type
        {
        public: 
            virtual void method() = 0;
        };

        class concrete_type : public abstract_type 
        {
        public:
            void method() override {}
        };

    public:
        #pragma region is_abstract<actual_t>()

        TEST_METHOD(is_abstract_fails_when_type_is_concrete)
        {
            assert::is_abstract<concrete_type>();

            Assert::AreNotEqual(wstring::npos, this->output.find(L"abstract"));
            Assert::AreNotEqual(wstring::npos, this->output.find(L"concrete_type"));
        }

        TEST_METHOD(is_abstract_doesnt_fail_when_type_is_abstract)
        {
            assert::is_abstract<abstract_type>();

            Assert::AreEqual<size_t>(0, this->output.length());
        }

        #pragma endregion

        #pragma region is_same<expected_t, actual_t>()
        
        TEST_METHOD(is_same_fails_when_types_are_different)
        {
            assert::is_same<int, double>();

            Assert::AreNotEqual(wstring::npos, this->output.find(L"int"));
            Assert::AreNotEqual(wstring::npos, this->output.find(L"double"));
        }

        TEST_METHOD(is_same_failure_message_includes_const_and_volatile_modifiers)
        {
            assert::is_same<volatile int, const int>();

            Assert::AreNotEqual(wstring::npos, this->output.find(L"volatile int"));
            Assert::AreNotEqual(wstring::npos, this->output.find(L"const int"));
        }

        #pragma endregion
    };
}