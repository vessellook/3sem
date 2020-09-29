#include <iostream>
#include <memory>

#include "../../asserts.h"

#include "../dynamic_array.h"
#define TestDynamicArray DynamicArray

//#include "../dynamic_array/safe_dynamic_array.h"
//#define TestDynamicArray SafeDynamicArray

using namespace std;
using namespace sem2;
using tests::assert_equal;

bool check_constructor_and_size() {
    auto array1 = TestDynamicArray<int>();
    assert_equal(array1.getSize(), (unsigned)0);
    auto array2 = TestDynamicArray<double>();
    assert_equal(array1.getSize(), (unsigned)0);
    auto array3 = TestDynamicArray<shared_ptr<int>>();
    assert_equal(array1.getSize(), (unsigned)0);
    auto array4 = TestDynamicArray<shared_ptr<double>>();
    assert_equal(array4.getSize(), (unsigned)0);
    auto array5 = TestDynamicArray<shared_ptr<TestDynamicArray<int>>>();
    assert_equal(array5.getSize(), (unsigned)0);
    auto array6 = TestDynamicArray<shared_ptr<TestDynamicArray<shared_ptr<int>>>>();
    assert_equal(array6.getSize(), (unsigned)0);

    auto array11 = TestDynamicArray<int>(5);
    assert_equal(array11.getSize(), (unsigned)5);
    auto array12 = TestDynamicArray<double>(5);
    assert_equal(array12.getSize(), (unsigned)5);
    auto array13 = TestDynamicArray<shared_ptr<int>>(5);
    assert_equal(array13.getSize(), (unsigned)5);
    auto array14 = TestDynamicArray<shared_ptr<double>>(5);
    assert_equal(array14.getSize(), (unsigned)5);
    auto array15 = TestDynamicArray<shared_ptr<TestDynamicArray<int>>>(5);
    assert_equal(array15.getSize(), (unsigned)5);
    auto array16 = TestDynamicArray<shared_ptr<TestDynamicArray<shared_ptr<int>>>>(5);
    assert_equal(array16.getSize(), (unsigned)5);

    int* int_array = new int[5];
    auto array21 = TestDynamicArray<int>(int_array, 5);
    assert_equal(array21.getSize(), (unsigned)5);
    auto* double_array = new double[5];
    auto array22 = TestDynamicArray<double>(double_array, 5);
    assert_equal(array22.getSize(), (unsigned)5);
    delete[] int_array;
    delete[] double_array;
    return true;
}

bool check_set_and_get() {
    auto array1 = TestDynamicArray<int>(10);
    array1.set(0, 3);
    array1.set(1, 5);
    array1.set(2, 0);
    assert_equal(array1.get(0), 3, __FILE__, __LINE__);
    assert_equal(array1.get(1), 5, __FILE__, __LINE__);
    assert_equal(array1.get(2), 0, __FILE__, __LINE__);
    array1.get(0) = 1;
    array1.get(1) = 2;
    array1.get(2) = 3;
    assert_equal(array1.get(0), 1, __FILE__, __LINE__);
    assert_equal(array1.get(1), 2, __FILE__, __LINE__);
    assert_equal(array1.get(2), 3, __FILE__, __LINE__);
    auto array3 = make_shared<TestDynamicArray<int>>(5);
    array3->set(0, 1);
    array3->set(1, 2);
    array3->set(2, 3);
    auto array4 = TestDynamicArray<shared_ptr<TestDynamicArray<int>>>(10);
    array4.set(5, array3);
    assert_equal(array4.get(5)->get(2), 3, __FILE__, __LINE__);
    array3->set(2, 4);
    assert_equal(array4.get(5)->get(2), 4, __FILE__, __LINE__);
    return true;
}

bool check_subscript_operator() {
    auto array1 = TestDynamicArray<int>(10);
    array1[0] = 3;
    array1[1] = 5;
    array1[2] = 0;
    assert_equal(array1[0], 3, __FILE__, __LINE__);
    assert_equal(array1[1], 5, __FILE__, __LINE__);
    assert_equal(array1[2], 0, __FILE__, __LINE__);
    array1[0] = 1;
    array1[1] = 2;
    array1[2] = 3;
    assert_equal(array1[0], 1, __FILE__, __LINE__);
    assert_equal(array1[1], 2, __FILE__, __LINE__);
    assert_equal(array1[2], 3, __FILE__, __LINE__);
    auto array3 = make_shared<TestDynamicArray<int>>(5);
    (*array3)[0] = 1;
    (*array3)[1] = 2;
    (*array3)[2] = 3;
    auto array4 = TestDynamicArray<shared_ptr<TestDynamicArray<int>>>(10);
    array4[5] = array3;
    assert_equal((*(array4[5]))[2], 3, __FILE__, __LINE__);
    (*array3)[2] = 4;
    assert_equal((*(array4[5]))[2], 4, __FILE__, __LINE__);
    return true;
}

bool check_resize() {
    auto array1 = TestDynamicArray<int>();
    array1.resize(3);
    assert_equal(array1.getSize(), (unsigned)3);
    array1.set(0, 1);
    array1.set(1, 2);
    array1.set(2, 3);
    assert_equal(array1.get(0), 1);
    assert_equal(array1.get(1), 2);
    assert_equal(array1.get(2), 3);
    array1.resize(5);
    array1.set(3, 4);
    array1.set(4, 5);
    assert_equal(array1.get(3), 4);
    assert_equal(array1.get(4), 5);
    return true;
}

int main() {
    check_constructor_and_size();
    cout << "constructor TEST PASSED" << endl;

    check_set_and_get();
    cout << "set&get TEST PASSED" << endl;
    check_subscript_operator();
    cout << "operator[] TEST PASSED" << endl;
    check_resize();
    cout << "resize TEST PASSED" << endl;

    auto *array1 = new TestDynamicArray<int>();
    assert_equal<unsigned>(array1->getSize(), 0);

    auto *array2 = new TestDynamicArray<int>(5);
    assert_equal<unsigned>(array2->getSize(), 5);

    array1->resize(10);
    assert_equal<unsigned>(array1->getSize(), 10);

    array1->set(0, 3);
    array1->set(3, 5);
    array1->set(5, 0);

    assert_equal(array1->get(0), 3);
    assert_equal(array1->get(3), 5);
    assert_equal(array1->get(5), 0);

    array2->set(0, 1);
    array2->set(1, 2);
    array2->set(2, 3);
    array2->set(3, 4);
    array2->set(4, 5);

    assert_equal(array2->get(0), 1);
    assert_equal(array2->get(1), 2);
    assert_equal(array2->get(2), 3);
    assert_equal(array2->get(3), 4);
    assert_equal(array2->get(4), 5);

    int *items = new int[5];
    items[0] = 1;
    items[1] = 2;
    items[2] = 4;
    items[3] = 8;
    items[4] = 16;

    auto *array3 = new TestDynamicArray<int>(items, 5);

    assert_equal(array3->get(0), 1);
    assert_equal(array3->get(1), 2);
    assert_equal(array3->get(2), 4);
    assert_equal(array3->get(3), 8);
    assert_equal(array3->get(4), 16);

    auto *array4 = new TestDynamicArray<int>(*array3);
    assert_equal(array4->get(0), 1);
    assert_equal(array4->get(1), 2);
    assert_equal(array4->get(2), 4);
    assert_equal(array4->get(3), 8);
    assert_equal(array4->get(4), 16);

    delete array1;
    delete array2;
    delete array3;
    delete array4;

    auto *array6 = new TestDynamicArray<unsigned *>();
    array6->resize(5);
    if (array6->getSize() < 5) {
        array6->resize(15);
    }
    for (unsigned i = 0; i < 5; ++i) {
        array6->set(i, new unsigned(i));
    }
    for (unsigned i = 0; i < 5; i++) {
        assert_equal(*array6->get(i), i);
    }

    delete array6->get(0);
    delete array6->get(1);
    delete array6->get(2);
    delete array6->get(3);
    delete array6->get(4);
    delete array6;

    cout << "FINISH";
    return 0;
}
