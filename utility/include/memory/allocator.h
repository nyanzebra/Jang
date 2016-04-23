#pragma once

#include <cstddef>
#include <type_traits>
#include <limits>
#include <new>
#include <assert.h>

namespace ftl {
namespace memory {

template<std::size_t N>
struct is_binary_power { enum { value = N & !(N & (N - 1)) }; };

template<typename T, std::size_t Alignment, typename = std::enable_if_t<std::is_pointer<T>::value && is_binary_power<Alignment>::value>>
struct aligner {
	typedef T pointer;
	typedef const T const_pointer;
	typedef std::ptrdiff_t difference_type;
	typedef std::size_t address_type;
	typedef std::size_t size_type;

	static pointer align(pointer address) { return (pointer)((reinterpret_cast<address_type>(address) + static_cast<address_type>(Alignment - 1)) & static_cast<address_type>(~(Alignment - 1))); }
	static difference_type adjust(const_pointer address) {
		size_type adjustment = Alignment - (reinterpret_cast<address_type>(address) & reinterpret_cast<address_type>(Alignment - 1));

		if (adjustment == Alignment) {
			return 0;
		}

		return adjustment;
	}
};

template<typename T>
struct allocator_traits {
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef void* void_pointer;
	typedef const void* const_void_pointer;
};


template<typename T, std::size_t Memory, std::size_t Alignment>
class  memory_policy {
public:
	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef void* void_pointer;
	typedef const void* const_void_pointer;

	static_assert(Memory > 0, "Memory to allocate must be a value greater than 0.");

	void address(size_type address) { _start_address = address; }

	void_pointer operator new(size_type size){
		try {
		if (_memory_tree.can_allocate(size)) {
			return _memory_tree.allocate(size);
		}
	return nullptr;
			 } catch (std::bad_alloc& ba) {
				 throw ba;
			 }
	}

		void_pointer operator new[](size_type size) {
		try {
			if (_memory_tree.can_allocate(size)) {
				return _memory_tree.allocate(size);
			}
			return nullptr;
		} catch (std::bad_alloc& ba) {
			throw ba;
		}
	}

		void operator delete(void_pointer ptr) noexcept {
		if (_memory_tree.can_deallocate((size_type)ptr)) {
			_memory_tree.deallocate();
		}
	}

	void operator delete[](void_pointer ptr) noexcept {
		if (_memory_tree.can_deallocate((size_type)ptr)) {
			_memory_tree.deallocate();
		}
	}

private:
	size_type _start_address;

	struct memory_node {
		memory_node(const size_type& address, const size_type& size) : address(address), size(size) {}
		memory_node(size_type&& address, size_type&& size) : address(std::move(address)), size(std::move(size)) {}
		memory_node(const memory_node& other) : address(other.address), size(other.size), reserved(other.reserved), left(other.left), right(other.right) {}
		memory_node(memory_node&& other) : address(std::move(other.address)), size(std::move(other.size)), reserved(std::move(other.reserved)), left(std::move(other.left)), right(std::move(other.right)) {
			other.left = nullptr;
			other.right = nullptr;
		}
		~memory_node() { if (left) { delete left; } if (right) { delete right; } }

		const bool is_full() { return reserved == size; }

		size_type address;
		size_type size;
		size_type reserved = 0;
		memory_node* left = nullptr, *right = nullptr;
	};

	class memory_tree {
	public:
		memory_tree() : root(new memory_node(_start_address, Memory)) {}
		memory_tree(const memory_tree&) = delete;
		memory_tree(memory_tree&&) = delete;
		~memory_tree() { if (temp) { delete temp; } delete root; }

		const bool can_allocate(size_type size) { return _can_deallocate(root, address); }

		const bool can_deallocate(size_type address) { return _can_deallocate(root, addresss); }

		void deallocate() {}

	private:
		const bool _can_allocate(memory_node* root, size_type size) {
			if (root) {

			}
		}

		const bool _can_deallocate(memory_node* root, size_type address) {
			if (root) {
				if (root->address == address) {
					temp = root;
					return true;
				}
				if (root->left) {
					if (root->left->address > address) {
						can_deallocate(root->left, address);
					}
				}
				if (root->right) {
					if (root->right->address < address) {
						can_deallocate(root->right, address);
					}
				}
			}
			return false;
		}


		void coalesce() {

		}
	private:
		memory_node* root = nullptr;
		memory_node* temp = nullptr;
	} _memory_tree;
};

#ifdef max
#undef max
template<typename T, std::size_t Memory = std::numeric_limits<std::size_t>::max(), std::size_t Alignment = __alignof(T), template<typename, std::size_t, std::size_t> typename Policy = memory_policy>
class allocator {
public:
	static_assert(!std::is_const<T>::value, "The C++ Standard forbids containers of const elements because allocator<const T> is ill-formed.");

	typedef Policy<T, Memory, Alignment> policy;

	typedef T value_type;
	typedef T& reference;
	typedef const T& const_reference;
	typedef T* pointer;
	typedef const T* const_pointer;
	typedef std::size_t size_type;
	typedef std::ptrdiff_t difference_type;
	typedef void* void_pointer;
	typedef const void* const_void_pointer;

	allocator() {}
	allocator(const allocator<T, Memory, Alignment, Policy>&) {}
	template<typename U>
	allocator(const allocator<U, Memory, Alignment, Policy>& alloc) {}
	allocator(allocator<T, Memory, Alignment, Policy>&&) {}
	~allocator() { assert(_allocations == 0 && _reserved == 0); free(_start_address); }

	template <typename U>
	allocator<value_type, Memory, Alignment, Policy>& operator=(const allocator<U, Memory, Alignment, Policy>&) { return *this; }

	template <typename U>
	allocator<value_type, Memory, Alignment, Policy>& operator=(allocator<U, Memory, Alignment, Policy>&&) { return *this; }

	template<typename U>
	struct rebind { typedef allocator<U, Memory, Alignment, Policy> other; };

	pointer address(reference value) const noexcept { return &value; }

	const_pointer address(const_reference value) const noexcept { return &value; }

	pointer allocate(size_type n) {
		assert(n > 0);
		if (n == 1) {
			return policy::operator new();
		} else {
			return policy::operator new[](n);
		}
	}

	pointer allocate(size_type n, const_void_pointer hint = 0) { return allocate(n); }

	void deallocate(pointer ptr, size_type n) {
		if (ptr && n > 0) {
			if (n == 1) {
				policy::operator delete(ptr);
			} else {
				policy::operator delete[](ptr);
			}
		}
	}

	template <typename Arg, typename... Args, typename = std::enable_if_t<std::is_pointer<Arg>::value && std::is_move_assignable<Args...>::value>>
	void construct(Arg ptr, Args... args) { new((void_pointer)ptr)value_type(std::forward<Args>(args)...); }

	template<typename Arg, typename = std::enable_if_t<std::is_pointer<Arg>::value>>
	void destroy(Arg ptr) { ptr->~value_type(); }

	const size_type max_size() const { return Memory / sizeof(value_type); }

	const size_type	alignment() const { return Alignment; }

	const size_type begin_address() const { return (size_type)_start_address; }

	const size_type end_address() const { return (size_type)(begin_address() + Memory); }

	template<typename A, typename B>
	friend bool operator==(const allocator<A>& a, const allocator<B>& b) { return true; }

	template<typename A, typename B>
	friend bool operator!=(const allocator<A>& a, const allocator<B>& b) { return !(a == b); }

private:
	pointer _allocate_array(size_type n) {
		size_type header = _header();
		if (sizeof(size_type) % sizeof(value_type) > 0) {
			header++;
		}
		pointer ptr = (pointer)_allocate_single()
	}

	void _deallocate_array(pointer array, size_type n) {
		for (size_type i = 0; i < n; i++) {
			array[i].~value_type();
		}
		size_type header = _header();
		if
	}

	size_type _header() { return sizeof(size_type) / sizeof(value_type); }

private:
	void_pointer _start_address = std::malloc(Memory);
	size_type _reserved = 0;
	size_type _allocations = 0;
};

#endif

}
}

