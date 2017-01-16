#pragma once

/*

The following code demonstrates how it can be used with the procps library where two pointers need to be managed PROCTAB and proc_t.

Two functions free_proc_tab and free_proc are defined to handle the specific cleanup operations associated with corresponding procps functions openproc and readproc.

Two MemoryManagement objects are created process_table and process_info using the cleanup operations. On destruction or assignment of new memory, the existing allocated memory will be properly de-allocated. Note that delete, delete[] and free() could be used where appropriate instead of the library specific API functions demonstrated here.

-----------------------------------------------------------------------------
#include <readproc.h>

template< typename PointerType >
void free_proc_tab( PointerType* ptr )
{
	closeproc( ptr );
}

template< typename PointerType >
void free_proc( PointerType* ptr )
{
	freeproc( ptr );
}


MemoryManager< PROCTAB, &free_proc_tab > process_table;
process_table = openproc( PROC_FILLARG | PROC_FILLSTAT | PROC_FILLSTATUS );

MemoryManager< proc_t, &free_proc > process_info;
process_info = readproc( process_table.get(), NULL );
-----------------------------------------------------------------------------

*/

namespace hdd::utility
{
	template<class PointerType, void deallocate(PointerType*)>
	class MemoryManager
	{
	public:
		MemoryManager() :
			pointer_(NULL)
		{
		}

		~MemoryManager()
		{
			if (pointer_ != NULL)
			{
				deallocate(pointer_);
			}
		}

		PointerType* get()
		{
			return pointer_;
		}

		MemoryManager& operator=(PointerType* pointer)
		{
			if (pointer_ != NULL)
			{
				deallocate(pointer_);
			}
			pointer_ = pointer;
			return *this;
		}

	private:
		// Private copy constructor - no implementation.
		MemoryManager(const MemoryManager&);

		// Private constructor - no implementation.
		MemoryManager(const PointerType&);

		// Private assignment operator - no implementation.
		MemoryManager& operator=(const MemoryManager&);

		// Raw managed pointer.
		PointerType* pointer_;
	};
}