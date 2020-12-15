#include <iostream>
#include <memory>

#include "../../asserts.h"

#include "../dynamic_array.h"

using namespace std;
using namespace sem2;
using tests::assert_equal;

bool check_constructor_and_size() {
    auto array1 = DynamicArray<int>();
    ASSERT_EQUAL(array1.getSize(), (unsigned)0);
    auto array2 = DynamicArray<double>();
    ASSERT_EQUAL(array1.getSize(), (unsigned)0);
    auto array3 = DynamicArray<shared_ptr<int>>();
    ASSERT_EQUAL(array1.getSize(), (unsigned)0);
    auto array4 = DynamicArray<shared_ptr<double>>();
    ASSERT_EQUAL(array4.getSize(), (unsigned)0);
    auto array5 = DynamicArray<shared_ptr<DynamicArray<int>>>();
    ASSERT_EQUAL(array5.getSize(), (unsigned)0);
    auto array6 = DynamicArray<shared_ptr<DynamicArray<shared_ptr<int>>>>();
    ASSERT_EQUAL(array6.getSize(), (unsigned)0);

    auto array11 = DynamicArray<int>(5);
    ASSERT_EQUAL(array11.getSize(), (unsigned)5);
    auto array12 = DynamicArray<double>(5);
    ASSERT_EQUAL(array12.getSize(), (unsigned)5);
    auto array13 = DynamicArray<shared_ptr<int>>(5);
    ASSERT_EQUAL(array13.getSize(), (unsigned)5);
    auto array14 = DynamicArray<shared_ptr<double>>(5);
    ASSERT_EQUAL(array14.getSize(), (unsigned)5);
    auto array15 = DynamicArray<shared_ptr<DynamicArray<int>>>(5);
    ASSERT_EQUAL(array15.getSize(), (unsigned)5);
    auto array16 = DynamicArray<shared_ptr<DynamicArray<shared_ptr<int>>>>(5);
    ASSERT_EQUAL(array16.getSize(), (unsigned)5);

    int* int_array = new int[5];
    auto array21 = DynamicArray<int>(int_array, 5);
    ASSERT_EQUAL(array21.getSize(), (unsigned)5);
    auto* double_array = new double[5];
    auto array22 = DynamicArray<double>(double_array, 5);
    ASSERT_EQUAL(array22.getSize(), (unsigned)5);
    delete[] int_array;
    delete[] double_array;
    return true;
}

bool check_set_and_get() {
    auto array1 = DynamicArray<int>(10);
    array1.set(0, 3);
    array1.set(1, 5);
    array1.set(2, 0);
    ASSERT_EQUAL(array1.get(0), 3);
    ASSERT_EQUAL(array1.get(1), 5);
    ASSERT_EQUAL(array1.get(2), 0);
    array1.get(0) = 1;
    array1.get(1) = 2;
    array1.get(2) = 3;
    ASSERT_EQUAL(array1.get(0), 1);
    ASSERT_EQUAL(array1.get(1), 2);
    ASSERT_EQUAL(array1.get(2), 3);
    auto array3 = make_shared<DynamicArray<int>>(5);
    array3->set(0, 1);
    array3->set(1, 2);
    array3->set(2, 3);
    auto array4 = DynamicArray<shared_ptr<DynamicArray<int>>>(10);
    array4.set(5, array3);
    ASSERT_EQUAL(array4.get(5)->get(2), 3);
    array3->set(2, 4);
    ASSERT_EQUAL(array4.get(5)->get(2), 4);
    return true;
}

bool check_subscript_operator() {
    auto array1 = DynamicArray<int>(10);
    array1[0] = 3;
    array1[1] = 5;
    array1[2] = 0;
    ASSERT_EQUAL(array1[0], 3);
    ASSERT_EQUAL(array1[1], 5);
    ASSERT_EQUAL(array1[2], 0);
    array1[0] = 1;
    array1[1] = 2;
    array1[2] = 3;
    ASSERT_EQUAL(array1[0], 1);
    ASSERT_EQUAL(array1[1], 2);
    ASSERT_EQUAL(array1[2], 3);
    auto array3 = make_shared<DynamicArray<int>>(5);
    (*array3)[0] = 1;
    (*array3)[1] = 2;
    (*array3)[2] = 3;
    auto array4 = DynamicArray<shared_ptr<DynamicArray<int>>>(10);
    array4[5] = array3;
    ASSERT_EQUAL((*(array4[5]))[2], 3);
    (*array3)[2] = 4;
    ASSERT_EQUAL((*(array4[5]))[2], 4);
    return true;
}

bool check_resize() {
    auto array1 = DynamicArray<int>();
    array1.resize(3);
    ASSERT_EQUAL(array1.getSize(), (unsigned)3);
    array1.set(0, 1);
    array1.set(1, 2);
    array1.set(2, 3);
    ASSERT_EQUAL(array1.get(0), 1);
    ASSERT_EQUAL(array1.get(1), 2);
    ASSERT_EQUAL(array1.get(2), 3);
    array1.resize(5);
    array1.set(3, 4);
    array1.set(4, 5);
    ASSERT_EQUAL(array1.get(3), 4);
    ASSERT_EQUAL(array1.get(4), 5);
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

    auto *array1 = new DynamicArray<int>();
    ASSERT_EQUAL(array1->getSize(), 0u);

    auto *array2 = new DynamicArray<int>(5);
    ASSERT_EQUAL(array2->getSize(), 5u);

    array1->resize(10);
    ASSERT_EQUAL(array1->getSize(), 10u);

    array1->set(0, 3);
    array1->set(3, 5);
    array1->set(5, 0);

    ASSERT_EQUAL(array1->get(0), 3);
    ASSERT_EQUAL(array1->get(3), 5);
    ASSERT_EQUAL(array1->get(5), 0);

    array2->set(0, 1);
    array2->set(1, 2);
    array2->set(2, 3);
    array2->set(3, 4);
    array2->set(4, 5);

    ASSERT_EQUAL(array2->get(0), 1);
    ASSERT_EQUAL(array2->get(1), 2);
    ASSERT_EQUAL(array2->get(2), 3);
    ASSERT_EQUAL(array2->get(3), 4);
    ASSERT_EQUAL(array2->get(4), 5);

    int *items = new int[5];
    items[0] = 1;
    items[1] = 2;
    items[2] = 4;
    items[3] = 8;
    items[4] = 16;

    auto *array3 = new DynamicArray<int>(items, 5);

    ASSERT_EQUAL(array3->get(0), 1);
    ASSERT_EQUAL(array3->get(1), 2);
    ASSERT_EQUAL(array3->get(2), 4);
    ASSERT_EQUAL(array3->get(3), 8);
    ASSERT_EQUAL(array3->get(4), 16);

    auto *array4 = new DynamicArray<int>(*array3);
    ASSERT_EQUAL(array4->get(0), 1);
    ASSERT_EQUAL(array4->get(1), 2);
    ASSERT_EQUAL(array4->get(2), 4);
    ASSERT_EQUAL(array4->get(3), 8);
    ASSERT_EQUAL(array4->get(4), 16);

    delete array1;
    delete array2;
    delete array3;
    delete array4;

    auto *array6 = new DynamicArray<unsigned *>();
    array6->resize(5);
    if (array6->getSize() < 5) {
        array6->resize(15);
    }
    for (unsigned i = 0; i < 5; ++i) {
        array6->set(i, new unsigned(i));
    }
    for (unsigned i = 0; i < 5; i++) {
        ASSERT_EQUAL(*array6->get(i), i);
    }

    auto j = 0u;
    for(auto i : *array6) {
        ASSERT_EQUAL(*i, j++);
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
