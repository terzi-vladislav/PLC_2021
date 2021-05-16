#include <cstdint>
#include <iostream>

template<typename X, typename Y>

struct IsDirectDescendant {
	enum : bool { val = false };
};

template<typename X>

struct IsDirectDescendant<X, X> {
	enum : bool { val = true };
};

template<typename...>

struct TypeList {};

template<typename X, typename Y>

struct IsDescendant {
	enum : bool { val = IsDirectDescendant<X, Y>::val || IsDescendant<typename X::ParentsList, Y>::val };
};

template<typename ...Ts, typename Y>

struct IsDescendant<TypeList<Ts...>, Y> {
	enum : bool { val = (IsDescendant<Ts, Y>::val || ... || false) };
};

using u64 = uint64_t;

struct TypeInfo {
	u64 type;
	const char* name;
};

constexpr bool isPrime(u64 x) {
	if (x == 2 || x == 3) {
		return true;
	}
	if (x % 2 == 0 || x % 3 == 0) {
		return true;
	}
	for (u64 i = 5, j = 2; i * i <= x; i += j, j ^= 6) {
		if (x % i == 0) {
			return false;
		}
	}
	return true;
}

constexpr u64 getNthPrime(u64 n) {
	u64 x = 2;
	while (n != 0) {
		x++;
		if (isPrime(x)) {
			n--;
		}
	}
	return x;
}

template <typename ...T>
struct Multiplier {
	enum : u64 { val = (T::type * ... * 1) };
};


struct Object {
	TypeInfo* vtable;
	explicit Object(TypeInfo* vtable) : vtable(vtable) {}
};


#define VIRTUAL_CLASS(name, ...)                                             \
TypeInfo name##TypeInfo { 0, #name };                                        \
struct name : __VA_OPT__(__VA_ARGS__,) virtual Object {                      \
public:                                                                      \
    using ParentsList = TypeList<__VA_ARGS__>;                               \
    enum : u64 {                                                             \
        prime = getNthPrime(__COUNTER__),                                    \
        type = prime * Multiplier<__VA_ARGS__>::val                          \
    };                                                                       \
    name() : Object(&name##TypeInfo) {}                                      \

#define END_CLASS(name)  }; static struct TypeInfoAssigner##name { TypeInfoAssigner##name() noexcept { name##TypeInfo.type = name::type; } } typeInfoAssigner##name;

template <typename r, typename S>
r DYNAMIC_CAST(S* ptr) {
	using R = typename std::remove_pointer<r>::type;
	if constexpr (IsDescendant<S, R>::val) {
		return static_cast<R*>(ptr);
	}
	if (ptr->vtable->type % R::type == 0 && ptr->vtable->type % (R::type * R::type) != 0) {
		return static_cast<R*>(ptr);
	}
	return nullptr;
};

#define TYPEID(x) (*(x)->vtable)

VIRTUAL_CLASS(A)
END_CLASS(A)


VIRTUAL_CLASS(C, A)
END_CLASS(C)


VIRTUAL_CLASS(D, C)
END_CLASS(D)


VIRTUAL_CLASS(E, C)
END_CLASS(E)


VIRTUAL_CLASS(F, D, E)
public:
	char x;
END_CLASS(F)


int main() {
	static_assert(IsDescendant<C, A>::val);
	static_assert(IsDescendant<D, A>::val);
	static_assert(IsDescendant<E, A>::val);
	static_assert(IsDescendant<F, A>::val);

	static_assert(IsDescendant<D, C>::val);
	static_assert(IsDescendant<E, C>::val);
	static_assert(IsDescendant<F, C>::val);

	static_assert(IsDescendant<F, D>::val);
	static_assert(IsDescendant<F, E>::val);

	static_assert(!IsDescendant<A, C>::val);
	static_assert(!IsDescendant<A, D>::val);
	static_assert(!IsDescendant<A, E>::val);
	static_assert(!IsDescendant<A, F>::val);
	static_assert(!IsDescendant<C, D>::val);
	static_assert(!IsDescendant<C, E>::val);
	static_assert(!IsDescendant<C, F>::val);
	static_assert(!IsDescendant<D, F>::val);
	static_assert(!IsDescendant<E, F>::val);

	A a;

	C c;

	D d;
	E e;

	F f;
	f.x = 'x';

	F* pf = &f;
	// up cast
	D* pd = DYNAMIC_CAST<D*>(pf);

	// down cast
	std::cout << DYNAMIC_CAST<F*>(pd) << '\n' << DYNAMIC_CAST<F*>(&d) << '\n';

	std::cout << TYPEID(pd).name << "\n";

	return 0;
}