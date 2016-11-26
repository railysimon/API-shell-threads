#include <iostream>
#include <stack>
#include <thread>
#include <mutex>

using namespace std;

stack<int> st;
mutex st_mutex;
const int size = 3;

void thread_function(const int id)
{
    st_mutex.lock();
    st.push(id);
    st_mutex.unlock();
}

int main()
{
    thread **threads = new thread*[size];
    for(int i=0; i<size; i++)
    {
        cout << "Create " << i+1 << " thread. Push to stack value: " << i+1 << endl;
        threads[i] = new thread(thread_function, (i+1));
    }

    for(int i=0; i<size; i++)
        threads[i]->join();

    for(int i=0; i<size; i++)
        delete threads[i];
    delete [] threads;

    int i = 1;
    cout << endl << "Stack values: " << endl;

    while(!st.empty())
    {
        cout << "[" << i++ << "]: " << st.top() << endl;
        st.pop();
    }

    cout << endl << "All values were deleted." << endl;

    return 0;
}
