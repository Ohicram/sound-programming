#pragma once

namespace FmodWrapperLibrary
{
	class FmodWrapper
	{
	public:
		static FmodWrapper& getInstance()
		{
			static FmodWrapper instance;
			return  instance;
		}
			static void SayHello();
			FmodWrapper(FmodWrapper const&) = delete;
			void operator=(FmodWrapper const&) = delete;
	private:
		FmodWrapper();
	};
}

