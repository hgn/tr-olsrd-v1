/*
 * The olsr.org Optimized Link-State Routing daemon (olsrd)
 *
 * (c) by the OLSR project
 *
 * See our Git repository to find out who worked on this file
 * and thus is a copyright holder on it.
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * * Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 * * Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in
 *   the documentation and/or other materials provided with the
 *   distribution.
 * * Neither the name of olsr.org, olsrd nor the names of its
 *   contributors may be used to endorse or promote products derived
 *   from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 * FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 * COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 * BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * Visit http://www.olsr.org for more information.
 *
 * If you find this software useful feel free to make a donation
 * to the project. For more information see the website or contact
 * the copyright holders.
 *
 */

#ifndef LISTBACKPORT_H_
#define LISTBACKPORT_H_

#include <stddef.h>
#include <stdbool.h>

#include "container_of.h"
#include "defs.h"

/**
 * this struct is used as list head and list elements.
 * the list-nodes and the list-head contain two rings of
 * pointers (one forward, one backward), the first/last node
 * have a link to the head, no NULL element.
 */
struct list_entity {
  /**
   * Pointer to next element in list or to list head if last element
   */
  struct list_entity *next;

  /**
   * Pointer to previous element in list or list head if first element
   */
  struct list_entity *prev;
};

/**
 * initialize a list-head
 * @param pointer to list-head
 */
static INLINE void listbackport_init_head(struct list_entity *head) {
  head->next = head->prev = head;
}

/**
 * initialize a list-node
 * @param pointer to list-node
 */
static INLINE void listbackport_init_node(struct list_entity *entity) {
  entity->next = entity->prev = NULL;
}

/**
 * internal function to add a new node between two other nodes.
 * @param prev node before the insertion point
 * @param next node after the insertion point
 * @param new node which will be added to the list between 'prev' and 'next'
 */
static INLINE void __listbackport_add(struct list_entity *prev, struct list_entity *next, struct list_entity *new) {
  new->next = next;
  new->prev = prev;
  next->prev = new;
  prev->next = new;
}

/**
 * adds a node at the beginning of a list
 * @param head pointer to list head
 * @param new node which will be added to the list
 */
static INLINE void listbackport_add_head(struct list_entity *head, struct list_entity *new) {
  __listbackport_add(head, head->next, new);
}

/**
 * adds a node at the end of a list
 * @param head pointer to list head
 * @param new node which will be added to the list
 */
static INLINE void listbackport_add_tail(struct list_entity *head, struct list_entity *new) {
  __listbackport_add(head->prev, head, new);
}

/**
 * adds a node before another node
 * @param before reference node in the list
 * @param new node which will be added to the list
 */
static INLINE void listbackport_add_before(struct list_entity *before, struct list_entity *new) {
  __listbackport_add(before->prev, before, new);
}

/**
 * adds a node after another node
 * @param before reference node in the list
 * @param new node which will be added to the list
 */
static INLINE void listbackport_add_after(struct list_entity *after, struct list_entity *new) {
  __listbackport_add(after, after->next, new);
}

/**
 * internal function that removes the nodes between two other nodes.
 * @param prev node before the removed part of the list
 * @param next node after the removed part of the list
 */
static INLINE void __listbackport_remove(struct list_entity *prev, struct list_entity *next) {
  prev->next = next;
  next->prev = prev;
}

/**
 * removes a node from a list and clears node pointers
 * @param entity node to remove from the list
 */
static INLINE void listbackport_remove(struct list_entity *entity) {
  __listbackport_remove(entity->prev, entity->next);
  entity->prev = entity->next = NULL;
}

/**
 * checks if list is empty
 * @param head pointer to list head
 * @return true if list is empty, false otherwise
 */
static INLINE bool listbackport_is_empty(struct list_entity *head) {
  return head->next == head && head->prev == head;
}

/**
 * checks if node has been added to a list
 * @param node pointer to node
 * @return true if both pointers of the node are initialized,
 *   false otherwise
 */
static INLINE bool listbackport_node_added(struct list_entity *node) {
  return node->next != NULL && node->prev != NULL;
}

/**
 * checks if a node is the first element of a list
 * @param head pointer to list head
 * @param entity pointer to node
 * @return true if node is first element of list, false otherwise
 */
static INLINE bool listbackport_is_first(const struct list_entity *head, const struct list_entity *entity) {
  return head->next == entity;
}

/**
 * checks if node is the last element of a list
 * @param head pointer to list head
 * @param entity pointer to node
 * @return true if node is last element of list, false otherwise
 */
static INLINE bool listbackport_is_last(const struct list_entity *head, const struct list_entity *entity) {
  return head->prev == entity;
}

/**
 * Merge two lists and clear the second head
 * @param add_to head of the list which will contain all elements
 * @param remove_from head of the list which elements will be added after the elements
 *   of the first one
 */
static INLINE void listbackport_merge(struct list_entity *add_to, struct list_entity *remove_from) {
  if (listbackport_is_empty(remove_from)) {
    return;
  }

  add_to->next->prev = remove_from->prev;
  remove_from->prev->next = add_to->next;
  add_to->next = remove_from->next;
  remove_from->next->prev = add_to;

  listbackport_init_head(remove_from);
}

/**
 * @param head pointer to list-head
 * @param element pointer to a node element
 *    (don't need to be initialized)
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @return pointer to the first element of the list
 *    (automatically converted to type 'element')
 */
#define listbackport_first_element(head, element, listbackport_member) \
    container_of((head)->next, typeof(*(element)), listbackport_member)

/**
 * @param head pointer to list-head
 * @param element pointer to a node element
 *    (don't need to be initialized)
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @return pointer to the last element of the list
 *    (automatically converted to type 'element')
 */
#define listbackport_last_element(head, element, listbackport_member) \
    container_of((head)->prev, typeof(*(element)), listbackport_member)

/**
 * This function must not be called for the last element of
 * a list
 *
 * @param element pointer to a node of the list
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @return pointer to the node after 'element'
 *    (automatically converted to type 'element')
 */
#define listbackport_next_element(element, listbackport_member) \
  container_of((&(element)->listbackport_member)->next, typeof(*(element)), listbackport_member)

/**
 * This function must not be called for the first element of
 * a list
 *
 * @param element pointer to a node of the list
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @return pointer to the node before 'element'
 *    (automatically converted to type 'element')
 */
#define listbackport_prev_element(element, listbackport_member) \
  container_of((&(element)->listbackport_member)->prev, typeof(*(element)), listbackport_member)

/**
 * Loop over a block of elements of a list, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 *
 * @param first_element first element of loop
 * @param last_element last element of loop
 * @param element iterator pointer to list element struct
 * @param listbackport_member name of list_entity within list element struct
 */
#define listbackport_for_element_range(first_element, last_element, element, listbackport_member) \
  for (element = (first_element); \
       element->listbackport_member.prev != &(last_element)->listbackport_member; \
       element = listbackport_next_element(element, listbackport_member))

/**
 * Loop over a block of elements of a list backwards, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 *
 * @param first_element first element of range (will be last returned by the loop)
 * @param last_element last element of range (will be first returned by the loop)
 * @param element iterator pointer to list element struct
 * @param listbackport_member name of list_entity within list element struct
 */
#define listbackport_for_element_range_reverse(first_element, last_element, element, listbackport_member) \
  for (element = (last_element); \
       element->listbackport_member.next != &(first_element)->listbackport_member; \
       element = listbackport_prev_element(element, listbackport_member))

/**
 * Loop over all elements of a list, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 *
 * @param head pointer to list-head
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 */
#define listbackport_for_each_element(head, element, listbackport_member) \
  listbackport_for_element_range(listbackport_first_element(head, element, listbackport_member), \
                         listbackport_last_element(head, element, listbackport_member), \
                         element, listbackport_member)

/**
 * Loop over all elements of a list backwards, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 *
 * @param head pointer to list-head
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 */
#define listbackport_for_each_element_reverse(head, element, listbackport_member) \
  listbackport_for_element_range_reverse(listbackport_first_element(head, element, listbackport_member), \
                                 listbackport_last_element(head, element, listbackport_member), \
                                 element, listbackport_member)

/**
 * Loop over a block of elements of a list, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 * The loop runs from the element 'first' to the end of the list.
 *
 * @param head pointer to head of list
 * @param first pointer to first element of loop
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 */
#define listbackport_for_element_to_last(head, first, element, listbackport_member) \
  listbackport_for_element_range(first, listbackport_last_element(head, element, listbackport_member), element, listbackport_member)

/**
 * Loop over a block of elements of a list backwards, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 * The loop runs from the end of the list to the element 'first'.
 *
 * @param head pointer to head of list
 * @param first pointer to first element of loop
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 */
#define listbackport_for_element_to_last_reverse(head, first, element, listbackport_member) \
  listbackport_for_element_range_reverse(first, listbackport_last_element(head, element, listbackport_member), element, listbackport_member)

/**
 * Loop over a block of elements of a list, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 * The loop runs from the start of the list to the element 'last'.
 *
 * @param head pointer to head of list
 * @param last pointer to last element of loop
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 */
#define listbackport_for_first_to_element(head, last, element, listbackport_member) \
  listbackport_for_element_range(listbackport_first_element(head, element, listbackport_member), last, element, listbackport_member)

/**
 * Loop over a block of elements of a list backwards, used similar to a for() command.
 * This loop should not be used if elements are removed from the list during
 * the loop.
 * The loop runs from the element 'last' to the start of the list.
 *
 * @param head pointer to head of list
 * @param last pointer to last element of loop
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @param loop_ptr pointer to an list_entity which is used as the
 *    internal iterator
 */
#define listbackport_for_first_to_element_reverse(head, last, element, listbackport_member) \
  listbackport_for_element_range_reverse(listbackport_first_element(head, element, listbackport_member), last, element, listbackport_member)

/**
 * Loop over a block of elements of a list, used similar to a for() command.
 * This loop can be used if the current element might be removed from
 * the list during the loop. Other elements should not be removed during
 * the loop.
 *
 * @param first_element first element of loop
 * @param last_element last element of loop
 * @param element iterator pointer to list element struct
 * @param listbackport_member name of list_entity within list element struct
 * @param ptr pointer to list element struct which is used to store
 *    the next node during the loop
 */
#define listbackport_for_element_range_safe(first_element, last_element, element, listbackport_member, ptr) \
  for (element = (first_element), ptr = listbackport_next_element(first_element, listbackport_member); \
       element->listbackport_member.prev != &(last_element)->listbackport_member; \
       element = ptr, ptr = listbackport_next_element(ptr, listbackport_member))

/**
 * Loop over a block of elements of a list backwards, used similar to a for() command.
 * This loop can be used if the current element might be removed from
 * the list during the loop. Other elements should not be removed during
 * the loop.
 *
 * @param first_element first element of range (will be last returned by the loop)
 * @param last_element last element of range (will be first returned by the loop)
 * @param element iterator pointer to list element struct
 * @param listbackport_member name of list_entity within list element struct
 * @param ptr pointer to list element struct which is used to store
 *    the previous node during the loop
 */
#define listbackport_for_element_range_reverse_safe(first_element, last_element, element, listbackport_member, ptr) \
  for (element = (last_element), ptr = listbackport_prev_element(last_element, listbackport_member); \
       element->listbackport_member.next != &(first_element)->listbackport_member; \
       element = ptr, ptr = listbackport_prev_element(ptr, listbackport_member))

/**
 * Loop over all elements of a list, used similar to a for() command.
 * This loop can be used if the current element might be removed from
 * the list during the loop. Other elements should not be removed during
 * the loop.
 *
 * @param head pointer to list-head
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @param ptr pointer to an list element struct which is used to store
 *    the next node during the loop
 */
#define listbackport_for_each_element_safe(head, element, listbackport_member, ptr) \
  listbackport_for_element_range_safe(listbackport_first_element(head, element, listbackport_member), \
                              listbackport_last_element(head, element, listbackport_member), \
                              element, listbackport_member, ptr)

/**
 * Loop over all elements of a list backwards, used similar to a for() command.
 * This loop can be used if the current element might be removed from
 * the list during the loop. Other elements should not be removed during
 * the loop.
 *
 * @param head pointer to list-head
 * @param element pointer to a node of the list, this element will
 *    contain the current node of the list during the loop
 * @param listbackport_member name of the list_entity element inside the
 *    larger struct
 * @param ptr pointer to an list element struct which is used to store
 *    the next node during the loop
 */
#define listbackport_for_each_element_reverse_safe(head, element, listbackport_member, ptr) \
  listbackport_for_element_range_reverse_safe(listbackport_first_element(head, element, listbackport_member), \
                                      listbackport_last_element(head, element, listbackport_member), \
                                      element, listbackport_member, ptr)

#endif /* LIST_H_ */
