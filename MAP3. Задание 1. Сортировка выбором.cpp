#include <iostream>
#include <list>
#include <iterator>
#include <future>
#include <thread>
#include <algorithm>

int min(std::list<int> m) 
{
    return *std::min_element(m.begin(), m.end());
}

void sort(std::list<int>& m, std::promise<std::string>_prom) 
{
    for (int i = 0; i < m.size(); i++)
    {
        auto begin = std::next(m.begin(), i);
        auto end = m.end();
        std::list<int> sub;
        sub.splice(sub.begin(), m, begin, end);
        std::future<int> result(std::async(std::launch::deferred, min, sub));

        int r = result.get();

        if (r != *sub.begin()) 
        {
            sub.insert(std::find(sub.begin(), sub.end(), r), *sub.begin());
            sub.pop_front();
            sub.remove(r);
            sub.push_front(r);
        }
        m.splice(end, sub);
    }
    _prom.set_value("Сортировка завершена! ");
}

void print(std::list<int>& m)
{
    for (const auto& elem : m)
        std::cout << elem << " ";
    std::cout << std::endl;
}

int main() 
{

    setlocale(LC_ALL, "Rus");

    std::list<int> list = { 15, 7, 2, 123, 67, 8, 4, 85, 1 };
    std::promise<std::string> prom;
    std::future<std::string> f = prom.get_future();

    print(list);

    std::thread t(sort, std::ref(list), std::move(prom));
    f.wait();
    std::cout << f.get() << std::endl;
    print(list);

    t.join();
}



