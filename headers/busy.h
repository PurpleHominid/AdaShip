#pragma once //ensure header only included once during compilation

class busy {
	public:
		busy();
		void show(int ms);

	protected:

	private:
		void interval();
		bool terminate_;
};