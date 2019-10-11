#include <iostream>
#include <memory>
#include <vector>
#include <type_traits>


template<typename T, typename Alloc = std::allocator<T>>
class my_vector
{
    private:
        using Traits = std::allocator_traits<Alloc>;
        using Fancy = typename Traits::pointer;
        Fancy m_vec;
        Fancy m_tmp;
        size_t N;
        Alloc alloc;
        
    public:
        using value_type =  T;
        explicit my_vector(size_t n):N(n)
        {
            if(N <= 0 ) throw std::out_of_range("Vector constructor has 0 size");
            m_vec = Traits::allocate(alloc, N);
            try
                {
                    T *raw_ptr = static_cast<T *>(m_vec);
                    for (size_t i = 0; i < N; i++)
                    {
                        Traits::construct(alloc, &raw_ptr[i], 0);
                    }
                }
            catch(...)
                {
                    Traits::deallocate(alloc, m_vec, N);
                }
            std::cout << "CONST" << std::endl;
        }
        explicit my_vector(my_vector& m_v):N(m_v.size())
        {
            if(N <= 0 ) throw std::out_of_range("Vector constructor has 0 size");
            m_vec = Traits::allocate(alloc, N);
            try
                {
                    T *raw_ptr = static_cast<T *>(m_vec);
                    for (size_t i = 0; i < N; i++)
                    {
                        Traits::construct(alloc, &raw_ptr[i], m_v.m_vec[i]);
                    }
                }
            catch(...)
                {
                    Traits::deallocate(alloc, m_vec, N);
                }
            std::cout << "CONST" << std::endl;
        }
        template<typename Vector>
        explicit my_vector(Vector& m_v):N(m_v.size())
        {
            m_vec = Traits::allocate(alloc, N);
            try
                {
                    T *raw_ptr = static_cast<T *>(m_vec);
                    for (size_t i = 0; i < N; i++)
                    {
                        Traits::construct(alloc, &raw_ptr[i], m_v[i]);
                    }
                }
            catch(...)
                {
                    Traits::deallocate(alloc, m_vec, N);
                }
            std::cout << "CONST_Vec" << std::endl;
        }
        ~my_vector(){
            clear();
            std::cout << "DEST" << std::endl;
            }
        void clear() noexcept 
        {
            if(m_vec)
            {
                for (size_t i = 0; i < N; i++)
                    {
                        Traits::destroy(alloc, &m_vec[i]);
                    }
                Traits::deallocate(alloc, m_vec, N);
                m_vec = nullptr;
            }
        }
        T& value()
        {
            my_vector tmp;
            return tmp[2];
        }
        const my_vector& operator = (const my_vector & v) 
        {
            for (size_t i = 0; i < v.size(); i++)
            {
                m_vec[i] = v.m_vec[i];
            }
            return *this;
        }
        template<typename Vector>
        my_vector& operator = (const Vector& that) 
        {
            for (size_t i = 0; i < N; i++)
            {
                m_vec[i] = that[i];
            }
            return *this;
        }

        T& operator()(int i){}
        T& operator[](int i) const { return m_vec[i];}
        int size() const { return N;}
        T operator()(int i) const {}
};
template<typename Scalar, typename Vector>
class scalar_times_vector_expresion
{
    public:
        using value_type = std::common_type_t<typename Vector::value_type>;
    private:
        Scalar s;
        const Vector& ref;
    public:
        scalar_times_vector_expresion(const Scalar& s, const Vector& v):s(s), ref(v) {}
        value_type operator[](int i) const
        {
            return s*ref[i];
        }
            
};
template<typename Scalar, typename Vector>
scalar_times_vector_expresion<Scalar, Vector> 
operator * (const Scalar& s, const Vector& v)
{
    return scalar_times_vector_expresion<Scalar, Vector>(s, v);
}
int main()
{
    const size_t N {12};
    my_vector<float> V(N);
    my_vector<float> K(N);
    
    for (size_t i = 0; i < N; i++)
    {
        V[i] = i*i+i;
        std::cout << V[i] << " ";
    }
    std::cout  << std::endl;
    my_vector<float> C(V);
   
    for (size_t i = 0; i < N; i++)
    {
        std::cout << C[i] << " ";
    }
    std::cout << "\n" << V.size() << std::endl;
    ////////////////////////!
    std::cout << "/*============================*/" << std::endl;
    std::vector<float> v;
    for (size_t i = 0; i < N; i++)
    {
        v.push_back(i*i);
        std::cout << v[i] << " ";
    }
    ////////////////////////!
    std::cout << "\n/*============================*/" << std::endl;
    my_vector<float> L(v);
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << L[i] << " ";
    }
    std::cout  << std::endl;
    std::cout << "\n/*============================*/" << std::endl;
    my_vector<float> D(v.size());
    D = v;
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << D[i] << " ";
    }
    std::cout  << std::endl;
    std::cout << "\n/*============================*/" << std::endl;
    my_vector<float> M(N);
    M = D;
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << M[i] << " ";
    }
    std::cout  << std::endl;
    std::cout << "\n/*============================*/" << std::endl;
    std::cout << "\n/*============================*/" << std::endl;
    V = 5.0*C;
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << V[i] << " ";
    }
    std::cout  << std::endl;
    V = 5.*(7.*C);
    for (size_t i = 0; i < v.size(); i++)
    {
        std::cout << V[i] << " ";
    }
    std::cout  << std::endl;
    return 0;
}
