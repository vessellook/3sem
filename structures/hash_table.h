#pragma once

#include <utility>
#include <functional>
#include <stdexcept>
#include <climits>
#include <memory>
#include <iostream>

#include "array_sequence.h"
#include "dictionary.h"

namespace sem3 {

    template<typename TElement>
    class HashTable {
        using Bucket = sem2::ArraySequence<TElement>;

        unsigned count = 0;
        sem2::ArraySequence<Bucket> buckets;
        std::function<unsigned(TElement)> hash;
    public:
        void resizeMore() {
            auto oldLength = buckets.getLength();
            auto newLength = oldLength * 2;
            for (auto bucketIndex = 0u; bucketIndex < oldLength; bucketIndex++) { buckets.append(Bucket()); }
            for (auto bucketIndex = 0u; bucketIndex < oldLength; bucketIndex++) {
                auto &oldBucket = buckets.get(bucketIndex);
                auto &newBucket = buckets.get(bucketIndex + oldLength);
                for (auto oldIndex = 0u; oldIndex < oldBucket.getLength();) {
                    if (hash(oldBucket.get(oldIndex)) / newLength == bucketIndex + oldLength) {
                        newBucket.append(std::move(oldBucket.get(oldIndex)));
                        oldBucket.remove(oldIndex);
                    } else {
                        oldIndex++;
                    }
                }
            }
        }

        void resizeLess() {
            auto oldLength = buckets.getLength();
            auto newLength = oldLength / 2;
            for (auto bucketIndex = 0u; bucketIndex < newLength; bucketIndex++) {
                auto &removingBucket = buckets.get(bucketIndex);
                auto &notRemovingBucket = buckets.get(bucketIndex + newLength);
                for (auto oldIndex = 0u; oldIndex < removingBucket.getLength();) {
                    notRemovingBucket.append(std::move(removingBucket.get(oldIndex)));
                }
            }
            for (auto bucketIndex = 0u; bucketIndex < newLength; bucketIndex++) { buckets.remove(0); }
        }

        explicit HashTable(unsigned buckets_count,
                           std::function<unsigned(TElement)> hash = std::hash<TElement>()) :
                count(0), buckets(buckets_count), hash(hash) {}

        class Iterator : public IIterator<TElement> {
            mutable std::unique_ptr<IIterator < Bucket>> outerIt;
            mutable std::unique_ptr<IIterator < TElement>> innerIt;

        public:

            explicit Iterator(const HashTable<TElement> *parent) : outerIt(parent->buckets.getIterator()),
                                                                   innerIt(parent->buckets.get(0).getIterator()) {
                outerIt->next();
            }

            const TElement &getCurrentItem() {
                try {
                    return innerIt->getCurrentItem();
                } catch (std::exception &e) {
                    throw std::runtime_error("No more items in HashMap!");
                }
            }

            [[nodiscard]] bool hasNext() const {
                if (innerIt->hasNext()) { return true; }
                while (outerIt->next()) {
                    innerIt = std::move(outerIt->getCurrentItem().getIterator());
                    if (innerIt->hasNext()) {
                        return true;
                    }
                }
                return false;
            }

            bool next() {
                if (innerIt->next()) { return true; }
                while (outerIt->next()) {
                    innerIt = outerIt->getCurrentItem().getIterator();
                    if (innerIt->next()) { return true; }
                }
                return false;
            }

            Pair<bool, const TElement *> tryGetCurrentItem() {
                return innerIt->tryGetCurrentItem();
            }
        };

        std::unique_ptr<IIterator < TElement>>
        getIterator() const { return std::make_unique<Iterator>(this); }

        bool remove(const TElement &element) {
            auto &bucket = getBucket(element);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (bucket[i] == element) {
                    bucket.remove(i);
                    count--;
                    return true;
                }
            }
            return false;
        }

        bool remove(unsigned elementHash, std::function<bool(const TElement &)> isMatch) {
            auto &bucket = getBucketByHash(elementHash);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (isMatch(bucket[i])) {
                    bucket.remove(i);
                    count--;
                    return true;
                }
            }
            return false;
        }

        bool add(TElement element) {
            auto &bucket = getBucket(element);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (bucket[i] == element) {
                    return false;
                }
            }
            bucket.append(std::move(element));
            count++;
            return true;
        }

        bool add(TElement element, std::function<bool(const TElement &)> isMatch) {
            auto &bucket = getBucket(element);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (isMatch(bucket[i])) {
                    bucket[i] = element;
                    return false;
                }
            }
            bucket.append(std::move(element));
            count++;
            return true;
        }

        const TElement &get(unsigned elementHash, std::function<bool(const TElement &)> isMatch) const {
            auto &bucket = getBucketByHash(elementHash);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (isMatch(bucket[i])) {
                    return bucket[i];
                }
            }
            throw std::runtime_error("no such element in HashTable");
        }

        TElement &get(unsigned elementHash, std::function<bool(const TElement &)> isMatch) {
            return const_cast<TElement &>(static_cast<const HashTable *>(this)->get(elementHash, isMatch));
        }

        bool contains(const TElement &element) const {
            auto &bucket = getBucket(element);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (bucket[i] == element) {
                    return true;
                }
            }
            return false;
        }

        bool contains(unsigned elementHash, std::function<bool(const TElement &)> isMatch) const {
            auto &bucket = getBucketByHash(elementHash);
            for (unsigned i = 0; i < bucket.getLength(); i++) {
                if (isMatch(bucket[i])) {
                    return true;
                }
            }
            return false;
        }

        [[nodiscard]] unsigned getCount() const {
            return count;
        }


        [[nodiscard]] unsigned getCapacity() const {
            return buckets.getLength();
        }

    private:
        Bucket &getBucket(const TElement &element) {
            return buckets[hash(element) % buckets.getLength()];
        }

        const Bucket &getBucket(const TElement &element) const {
            return buckets[hash(element) % buckets.getLength()];
        }

        Bucket &getBucketByHash(unsigned elementHash) {
            return buckets[elementHash % buckets.getLength()];
        }

        const Bucket &getBucketByHash(unsigned elementHash) const {
            return buckets[elementHash % buckets.getLength()];
        }
    };
}
