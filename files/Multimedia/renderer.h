#pragma once

namespace SWIFT
{
	class RENDERER
	{
		class IMPL;

	public:
		~RENDERER();

		void begin(struct BACKEND_WINDOW&);
		void stop();

	private:
		IMPL* m_impl = nullptr;
	};
}