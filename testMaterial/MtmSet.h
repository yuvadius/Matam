#ifndef MTM4_SET_H
#define MTM4_SET_H

#include <iostream>
#include "exceptions.h"

namespace mtm{
    
    /**
     * A set
     * @tparam Type The type of an element on the group.
     *      Required to implement:
     *                             - operator ==
     *                             - copy constructor
     */
    template<typename Type>
    class MtmSet{
        /**
         * A node in the set
         */
        class Node{
        public:
        	const Type data;
            Node* next;

            Node(Type data) : data(data) {
            	next = NULL;
            }
        };

        Node* head;
    
    public:
        //Forward declaration
        class const_iterator;
        
        /**
         * A iterator for Set
        */
        class iterator{
        public:
        	Node* index;

            /**
             * Empty constructor. Should not be dereferenced.
             * Same as MtmSet::end()
             */
            iterator() {
            	index = NULL;
            }
            
            /**
             * Constructor of Set iterator
             * @param node The node the iterator points to
             */
            explicit iterator(Node *node) {
            	index = node;
            }
            
            /**
             * Copy constructor
             * @param it The iterator to copy
             */
            iterator(const iterator& it) {
            	index = it.index;
            }

            /**
             * Destructor
             */
            ~iterator() {

            }

            /**
             * Copy assignment operator.
             * @param rhs The iterator to copy.
             * @return A reference to this iterator.
             */
            iterator& operator=(const iterator& rhs) {
            	index = rhs.index;
            	return *this;
            }
            
            /**
             * Dereference operator * .
             * Used like dereference of a pointer with * .
             * @return A reference of the data in the node the iterator
             *  points to.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type& operator*() const {
            	if(index == NULL) {
            		throw NodeIsEndException();
            	}
            	return index->data;
            }
            
            /**
             * Dereference operator -> .
             * Used like dereference of a pointer with -> .
             * @return A pointer to the data in the node the iterator
             *  points to.
             * C++ use the -> operator on the returned pointer,
             * this allows the user to treat the iterator like a pointer.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type *operator->() const {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
				return &(index->data);
            }
            
            /**
             * Prefix increment operator (++i)
             * @return a reference to the iterator;
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            iterator& operator++() {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
				index = index->next;
				return *this;
            }
            
            /**
             * Postfix increment operator (i++)
             * @return iterator that points to the same node as this before
             * the increment
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            iterator operator++(int) {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
            	iterator old(*this);
				index = index->next;
				return old;
            }
            
            /**
             * Compare an iterator with const_iterator. (when there is a
             *  regular iterator on the left.
             * @param rhs the right const_iterator.
             * @return true if the two iterators point to the same node
             */
            bool operator==(const const_iterator& rhs) const {
            	return (rhs == *this);
            }
            
            /**
             * Compare an iterator with const_iterator. (when there is a
             *  regular iterator on the left.
             * @param rhs the right const_iterator.
             * @return true if the two iterators don't point to the same node
             */
            bool operator!=(const const_iterator& rhs) const {
            	return (rhs != *this);
            }

            friend class const_iterator;
        };
        
        /**
         * A const_iterator for Set
        */
        class const_iterator {
        private:
        	Node* index;

        public:
            /**
             * Empty constructor. Should not be dereferenced.
             * Same as MtmSet::end()
             */
            const_iterator() {
            	index = NULL;
            }
            
            /**
             * Constructor of Set const_iterator
             * @param node The node the const_iterator points to
             */
            explicit const_iterator(Node *node) {
            	index = node;
            }
            
            /**
             * Copy constructor
             * @param it The const_iterator to copy
             */
            const_iterator(const const_iterator& it) {
            	index = it.index;
            }
            
            /**
             * Constructor from iterator (not const_iterator)
             * Allows casting from iterator to const_iterator
             * @param it The iterator to "copy" to a const_iterator
             */
            const_iterator(const iterator& it) {
            	index = it.index;
            }

            /**
             * Destructor
             */
            ~const_iterator() {

            }

            /**
             * Copy assignment operator.
             * @param rhs The iterator to copy.
             * @return A reference to this const_iterator.
             */
            const_iterator& operator=(const const_iterator& rhs) {
            	index = rhs.index;
				return *this;
            }
            
            /**
             * Dereference operator * .
             * Used like dereference of a pointer with * .
             * @return A reference of the data in the node the const_iterator
             *  points to.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type& operator*() const {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
				return index->data;
            }
            
            /**
             * Dereference operator -> .
             * Used like dereference of a pointer with -> .
             * @return A pointer to the data in the node the const_iterator
             *  points to.
             *  C++ use the -> operator on the returned pointer,
             *  this allows the user to treat the const_iterator like a pointer.
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const Type *operator->() const {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
            	return &(index->data);
            }
            
            /**
             * Prefix increment operator (++i)
             * @return a reference to the iterator;
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const_iterator& operator++() {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
            	index = index->next;
				return *this;
            }
            
            /**
             * Postfix increment operator (i++)
             * @return const_iterator that points to the same node as this
             * before the increment
             * @throws NodeIsEndException if the iterator doesn't point to
             * an element in the set (end())
             */
            const_iterator operator++(int) {
            	if(index == NULL) {
					throw NodeIsEndException();
				}
				iterator old(*this);
				index = index->next;
				return old;
            }
            
            /**
             * Compare two const_iterators.
             * @param rhs the right const_iterator
             * @return true if the two const_iterators point to the same node
             */
            bool operator==(const const_iterator& rhs) const {
            	if(index == NULL && rhs.index == NULL) {
					return true;
				}
            	else if (index == NULL || rhs.index == NULL) {
					return false;
				}
				else if(*(*this) == *rhs) {
					return true;
				}
				else {
					return false;
				}
            }
            
            /**
             * Compare two const_iterators.
             * @param rhs the right const_iterator
             * @return true if the two const_iterators don't point to the same
             * node
             */
            bool operator!=(const const_iterator& rhs) const {
            	return !(rhs == *this);
            }
        };
        
        /**
         * Empty constructor
         * Creates an empty set
         */
        MtmSet() {
        	head = NULL;
        }
        
        /**
         * Copy constructor
         * @param set the Set to copy
         */
        MtmSet(const MtmSet& set) {
			head = NULL;
			for(const_iterator it = set.begin(); it != set.end(); ++it) {
				insert(*it);
			}
        }
        
        /**
         * Destructor
         * Free all allocated memory in the set.
         */
        ~MtmSet() {
        	clear();
        }
        
        /**
         * Insert a new element to the set, doesn't insert if there is already
         *  an equal element in the set.
         * If an element was't inserted, all iterators should stay valid.
         * @param elem The element to insert to the set
         * @return An iterator to the inserted element, or to the equal
         * element if the element wasn't inserted.
         */
        iterator insert(const Type& elem) {
        	if(contains(elem) == false) {
				Node* new_head = new Node(elem);
				new_head->next = head;
				head = new_head;
				return begin();
			}
        	else {
        		return find(elem);
        	}
        }
        
        /**
         * Remove an element from the set. If there is no element equal to
         *  elem in the set, does nothing.
         * If an element wasn't removed, all iterators should stay valid.
         * @param elem the element to remove.
         */
        void erase(const Type& elem) {
        	if(contains(elem)) {
        		erase(find(elem));
        	}
        }
        
        /**
         * Remove the element the iterator points to from the set.
         * Iterator to another set is undefined.
         * @param it The iterator to the element to the set.
         */
        void erase(const iterator& it) {
        	if(contains(*it)) {
				iterator delete_element = find(*it);
				if(delete_element == begin()) {
					head = head->next;
					delete delete_element.index;
				}
				else {
					iterator prev;
					for(iterator iter = begin();iter != delete_element;++iter) {
						prev = iter;
					}
					prev.index->next = delete_element.index->next;
					delete delete_element.index;
				}
			}
        }
        
        /**
         * Get the amount of elements in the set.
         * @return The amount of elements in the set.
         */
        int size() const {
        	int counter = 0;
        	for(const_iterator it = begin(); it != end(); ++it) {
        		++counter;
        	}
        	return counter;
        }
        
        /**
         * Check if the set is empty.
         * @return true is the set is empty.
         */
        bool empty() const {
        	return (head == NULL);
        }
        
        /**
         * Empty the set, free all allocated memory in the set.
         */
        void clear() {
        	while(empty() == false) {
        		erase(head->data);
        	}
        }
        
        /**
         * Get an iterator to the first element in the set.
         * If set is empty, return the same as end.
         * @return
         */
        iterator begin() {
        	if(empty() == false) {
				iterator start(head);
				return start;
        	}
        	else {
        		return end();
        	}
        }
        
        /**
         * Get a const_iterator to the first element in the set.
         * If set is empty, return the same as end.
         * @return
         */
        const_iterator begin() const {
        	if(empty() == false) {
				const_iterator start(head);
				return start;
        	}
			else {
				return end();
			}
        }
        
        /**
         * Returns an iterator referring to the past-the-end element in the set.
         * The past-the-end element is the theoretical element that would follow
         * the last element in the set. It does not point to any element, and
         * thus should not be dereferenced.
         * @return Iterator to past-the-end element.
         */
        iterator end() {
        	iterator empty_iterator;
        	return empty_iterator;
        }
        
        /**
         * Returns a const_iterator referring to the past-the-end element in
         * the set.
         * The past-the-end element is the theoretical element that would follow
         * the last element in the set. It does not point to any element, and
         * thus should not be dereferenced.
         * @return const_iterator to past-the-end element.
         */
        const_iterator end() const {
        	const_iterator empty_iterator;
        	return empty_iterator;
        }
        
        /**
         * Find an element in the set.
         * @param elem The element to find
         * @return An iterator that points to the elem.
         */
        iterator find(const Type& elem) {
        	for(iterator it = begin(); it != end(); ++it) {
				if(*it == elem) {
					return it;
				}
			}
			return end();
        }
        
        /**
         * Find an element in the set.
         * @param elem The element to find
         * @return A const_iterator that points to the elem.
         */
        const_iterator find(const Type& elem) const {
        	for(const_iterator it = begin(); it != end(); ++it) {
				if(*it == elem) {
					return it;
				}
			}
			return end();
        }
        
        /**
         * Check if an element is in the set.
         * @param elem The element to check if it's in the set.
         * @return True if the element is in the set, false otherwise.
         */
        bool contains(const Type& elem) const {
        	if(find(elem) == end()) {
        		return false;
        	}
        	else {
        		return true;
        	}
        }
        
        /**
         * Check if this set is a superset of a given set.
         * Superset: A superset of B if B is a subset of A, that mean's that
         *  A contains all the elements in B.
         * @param subset The set to check if it's a subset.
         * @return True if the given set is a subset of this set.
         */
        bool isSuperSetOf(const MtmSet& subset) const {
        	for(const_iterator it = subset.begin(); it != subset.end(); ++it) {
				if(contains(*it) == false) {
					return false;
				}
			}
			return true;
        }
        
        /**
         * Check if two set are equal, meaning, they contain the same elements.
         * @param rhs The set right of the == operator.
         * @return true if thw two set conatain the same elements, false
         *  otherwise.
         */
        bool operator==(const MtmSet& rhs) const {
        	if(isSuperSetOf(rhs) && rhs.isSuperSetOf(*this)) {
        		return true;
        	}
        	else {
        		return false;
        	}
        }
        
        /**
         * Check if two set are equal, meaning, they contain the same elements.
         * @param rhs The set right of the != operator.
         * @return false if thw two set conatain the same elements, true
         *  otherwise.
         */
        bool operator!=(const MtmSet& rhs) const {
        	return !(*this == rhs);
        }
        
        /**
         * Insert all the elements in the given set to this set (union).
         * @param set The set to insert all the elements from.
         * @return A reference to this set.
         */
        MtmSet& unite(const MtmSet& set) {
			for(const_iterator it = set.begin(); it != set.end(); ++it) {
				if(contains(*it) == false) {
					insert(*it);
				}
			}
			return *this;
        }
        
        /**
         * Returns a new set that is an union of this set, and a given set.
         * @param set The other set (other than this) to be in the union.
         * @return The new set.
         */
        MtmSet unite(const MtmSet& set) const {
        	MtmSet new_set(*this);
        	return new_set.unite(set);
        }
        
        /**
         * Remove all the elements from this set, that are not in the given set
         * (intersection).
         * @param set The other set in the intersection.
         * @return A reference to this set.
         */
        MtmSet& intersect(const MtmSet& set) {
        	bool removed = true;
        	while(removed) {
        		removed = false;
				for(iterator it = begin(); it != end(); ++it) {
					if(set.contains(*it) == false) {
						erase(*it);
						removed = true;
						break;
					}
				}
        	}
			return *this;
        }
        
        /**
         * Returns a new set that is an intersection of this set,
         *  and a given set.
         * @param set The other set (other than this) to be in the intersection.
         * @return The new set.
         */
        MtmSet intersect(const MtmSet& set) const {
        	MtmSet new_set(*this);
			return new_set.intersect(set);
        }
        
        /**
         * Remove all the elements in the set, that doesn't meet a given
         *  condition.
         * @tparam func - A function of an object-function that receive 1
         *  argument, of the same type as an element in the set, and returns
         *  a bool.
         * @param condition - function (or object-function) that returns true
         * if the element is meeting the condition and false otherwise.
         * @return A reference to this set.
         */
        template<typename func>
        MtmSet& getSubSet(func condition) {
        	bool removed = true;
			while(removed) {
				removed = false;
				for(iterator it = begin(); it != end(); ++it) {
					if(condition(*it) == false) {
						erase(*it);
						removed = true;
						break;
					}
				}
			}
			return *this;
        }
        
        /**
         * Get a subset of this set, that contains all the elements in the set,
         * that doesn't meet a given condition.
         * @tparam func - A function of an object-function that receive 1
         *  argument, of the same type as an element in the set, and returns
         *  a bool.
         * @param condition - function (or object-function) that returns true
         * if the element is meeting the condition and false otherwise.
         * @return A the new set.
         */
        template<typename func>
        MtmSet getSubSet(func condition) const {
        	MtmSet new_set(*this);
			return new_set.getSubSet(condition);
        }
    };
} // namespace mtm

#endif //MTM4_SET_H
