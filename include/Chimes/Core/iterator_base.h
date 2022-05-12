// 12/05/2022 by BKHao in Chimes.

namespace Chimes
{
	// The base class of iterator, generating iterator of different objects.
	template<typename Object>
	class IteratorBase
	{
	public:
		IteratorBase()
		{

		}
		// The next item, preincrement.
		virtual IteratorBase& operator++() = 0;
		// The next item, postincrement.
		virtual IteratorBase operator++(int) = 0;
		// Access the current item.
		virtual Object& operator*() = 0;
		// Access the current item as a pointer.
		virtual Object* operator->() = 0;
	};
} // namespace Chimes
