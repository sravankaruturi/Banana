#pragma once

#include "RenderCommandQueue.h"
#include "RendererAPI.h"

namespace ee
{
	namespace re // re is rendering engine.
	{
		
		class Renderer
		{
			
		private:

			static Renderer* s_Instance;
			RenderCommandQueue m_CommandQueue;

		public:

			// TODO: Defined in multiple files. Consolidate that.
			typedef void(*RenderCommandFn)(void*);

			static void Clear();
			static void Clear(float r, float g, float b, float a = 1.f);
			static void SetClearColour(float r, float g, float b, float a);

			static void DrawIndexed(euint count);

			static void ClearMagenta();

			static void Init();

			static void* Submit(RenderCommandFn fn, euint size)
			{
				return s_Instance->m_CommandQueue.Allocate(fn, size);
			}

			void WaitAndRender();

			inline static Renderer& GetInstance() { return *s_Instance; }

		};

#pragma region Render Macros


#define EE_RENDER_PASTE2(a, b) a ## b
#define EE_RENDER_PASTE(a, b) EE_RENDER_PASTE2(a, b)
#define EE_RENDER_UNIQUE(x) EE_RENDER_PASTE(x, __LINE__)


/* Generate a new Unique struct for every Render Unique Call by using the line number and then submit those calls.
For example, a Call at line 52 would create a new struct, EERenderCommand52 and Use that struct to submit the command.

		The new mem line doesn't allocate memory, but calls the constructor for the unique struct.
		http://www.cplusplus.com/reference/new/operator%20new/

*/
#define EE_RENDER(code)\
	struct EE_RENDER_UNIQUE(EERenderCommand)\
	{\
		static void Execute(void*)\
		{\
			code\
		}\
	};\
	{\
		auto mem = ::ee::re::Renderer::Submit(EE_RENDER_UNIQUE(EERenderCommand)::Execute, sizeof(EE_RENDER_UNIQUE(EERenderCommand)));\
		new (mem) EE_RENDER_UNIQUE(EERenderCommand)();\
	}\


/**
 * \brief Submit a RenderAPI call with one argument.
 * \param arg0 argument
 * \param code API call.
 * 
 * Initialize a struct containing a variable of the same type as arg0 without references or consts. We would copy the variable value with the same name so that the code can run without any refactoring.
 */
#define EE_RENDER_1(arg0, code) \
	do {\
    struct EE_RENDER_UNIQUE(EERenderCommand) \
    {\
		EE_RENDER_UNIQUE(EERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0) \
		: arg0(arg0) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg0;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
    };\
	{\
		auto mem = ::ee::re::Renderer::Submit(EE_RENDER_UNIQUE(EERenderCommand)::Execute, sizeof(EE_RENDER_UNIQUE(EERenderCommand)));\
		new (mem) EE_RENDER_UNIQUE(EERenderCommand)(arg0);\
	} } while(0)


/**
 * \brief Renderer API call with two arguments
 * \param arg0 Argument 1
 * \param arg1 Argument 2
 * \param code API Call
 */
#define EE_RENDER_2(arg0, arg1, code) \
    struct EE_RENDER_UNIQUE(EERenderCommand) \
    {\
		EE_RENDER_UNIQUE(EERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1) \
		: arg0(arg0), arg1(arg1) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg1;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(EE_RENDER_UNIQUE(EERenderCommand)::Execute, sizeof(EE_RENDER_UNIQUE(EERenderCommand)));\
		new (mem) EE_RENDER_UNIQUE(EERenderCommand)(arg0, arg1);\
	}\

#define EE_RENDER_3(arg0, arg1, arg2, code) \
    struct EE_RENDER_UNIQUE(EERenderCommand) \
    {\
		EE_RENDER_UNIQUE(EERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2) \
		: arg0(arg0), arg1(arg1), arg2(arg2) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg2;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
    };\
	{\
		auto mem = ::Hazel::Renderer::Submit(EE_RENDER_UNIQUE(EERenderCommand)::Execute, sizeof(EE_RENDER_UNIQUE(EERenderCommand)));\
		new (mem) EE_RENDER_UNIQUE(EERenderCommand)(arg0, arg1, arg2);\
	}\

#define EE_RENDER_4(arg0, arg1, arg2, arg3, code) \
    struct EE_RENDER_UNIQUE(EERenderCommand) \
    {\
		EE_RENDER_UNIQUE(EERenderCommand)(typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2,\
											typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3)\
		: arg0(arg0), arg1(arg1), arg2(arg2), arg3(arg3) {}\
		\
        static void Execute(void* argBuffer)\
        {\
			auto& arg0 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg0;\
			auto& arg1 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg1;\
			auto& arg2 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg2;\
			auto& arg3 = ((EE_RENDER_UNIQUE(EERenderCommand)*)argBuffer)->arg3;\
            code\
        }\
		\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg0)>::type>::type arg0;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg1)>::type>::type arg1;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg2)>::type>::type arg2;\
		typename ::std::remove_const<typename ::std::remove_reference<decltype(arg3)>::type>::type arg3;\
    };\
	{\
		auto mem = Renderer::Submit(EE_RENDER_UNIQUE(EERenderCommand)::Execute, sizeof(EE_RENDER_UNIQUE(EERenderCommand)));\
		new (mem) EE_RENDER_UNIQUE(EERenderCommand)(arg0, arg1, arg2, arg3);\
	}

#define EE_RENDER_S(code) auto self = this;\
	EE_RENDER_1(self, code)

#define EE_RENDER_S1(arg0, code) auto self = this;\
	EE_RENDER_2(self, arg0, code)

#define EE_RENDER_S2(arg0, arg1, code) auto self = this;\
	EE_RENDER_3(self, arg0, arg1, code)

#define EE_RENDER_S3(arg0, arg1, arg2, code) auto self = this;\
	EE_RENDER_4(self, arg0, arg1, arg2, code)

#pragma endregion




	}
}