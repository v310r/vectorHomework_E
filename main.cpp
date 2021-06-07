#include <iostream>
#include <cassert>

template<class T>
class WeakVector{
private:
    T* arr;
    size_t length;
    size_t capacity;

    void checkAllocation(){
        if(length >= capacity){ 
            reserve(length * 2);
            if(arr)
                delete[] arr;
            arr = new T[capacity];
            }
    }
    T* createTMP(){
        T* tmp = new T[length];
        for(int i = 0;i < length;++i){
            tmp[i]=arr[i];
        }
        return tmp;
    }
public:
    ~WeakVector(){
        delete[] arr;
    }
    WeakVector(): arr(nullptr),length(0),capacity(0)
    {

    }
    WeakVector(size_t n): arr(nullptr),length(n),capacity(n)
    {
        checkAllocation();
        for(int i = 0;i < length;++i){
            arr[i] = 0;
        }
    }
    WeakVector(const WeakVector<T>& v):capacity(v.capacity){
        if(v.arr){
            length = v.length;
            arr = new T[length];
            for(int i = 0;i < length;++i)
                arr[i] = v.arr[i];
        }   
    }
    //MOVE SEMANTIC
    WeakVector(WeakVector<T>&& v):capacity(v.capacity){
        arr = v.arr;
        v.arr = nullptr;
        length = v.length;
    }
    size_t size() const
    {
        return length;
    }
    void resize(size_t n){
        int prevLength = length;
        if(n == 0) clear();
        else{
            length = n;
            checkAllocation();
            for(int i = prevLength - 1;i<length;++i){
            arr[i] = 0;
            }
        }

    }
    void clear(){
        delete[] arr;
        arr = nullptr;
        length = 0;
    }
    const T operator[](const size_t index)const{
        assert(index >=0 && index < length);
        return arr[index];
    }
    T& operator[](const size_t index){
        assert(index >=0 && index < length);
        return arr[index];
    }
    //MOVE SEMANTIC
    WeakVector<T>& operator=(WeakVector<T>&& vec){
        length = vec.length;
        capacity = vec.capacity;
        delete[] arr;
        arr = vec.arr;
        vec.arr = nullptr;
        return *this;
    }
    WeakVector<T>& operator=(const WeakVector<T>& vec){
        length = vec.length;
        checkAllocation();
        for(int i = 0;i < length;++i){
            arr[i] = vec.arr[i];
        }
        return *this;
    }
    void push_back(const T& value){
        T* tmp = createTMP();
        ++length;
        checkAllocation();
        for(int i = 0;i < length-1;++i){
            arr[i] = tmp[i];
        }
        arr[length-1] = value;
    }
    void pop_back(){
        T* tmp = createTMP();
        --length;
        checkAllocation();
        for(int i = 0;i < length-1;++i){
            arr[i] = tmp[i];
        }
    }
    void reserve(const size_t n){
        capacity = n;
    }
    size_t showCapacity(){
        return capacity;
    }
};


int main(){
    WeakVector<int> vec(10);
    std::cout<<vec.size()<<"\n\n";
    vec.resize(20);
   for(int i = 0;i< 20;++i){
       vec[i] = i;
       std::cout<<vec[i]<<" ";
    }
    std::cout<<"\n\n";
    WeakVector<int> vec2;
    vec2 = vec;
    WeakVector<int> vec3(vec2);
    vec3.push_back(20);
    vec3.push_back(30);
    vec3.pop_back();
    std::cout<<vec3[vec3.size()-1]<<"\n\n";
    std::cout<< vec3.showCapacity()<<"\n\n";

    //std::cout<<vec3[30]<<"\n\n"; //ERROR handled with assert() function

    WeakVector<int> tmp(std::move(vec2));
    tmp.pop_back();
    tmp = std::move(vec3);
    std::cout<<tmp[tmp.size()-1]<<"\n\n";
    return 0;
}