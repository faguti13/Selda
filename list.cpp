#include "list.h"
#include <iostream>

t_Node* node_create(void* data) {
    t_Node* temp_node = new t_Node;

    if (temp_node != NULL) {
        temp_node->data = data;
        temp_node->next = NULL;
        temp_node->previous = NULL;
        return temp_node;
    } else {
        std::cout << "Failed malloc Node\n";
        return NULL;
    }
}

t_List* list_create() {
    t_List* list = new t_List;

    if (list == NULL) {
        std::cout << "Failed malloc List\n";
        return NULL;
    }

    list->head = NULL;
    list->tail = NULL;
    list->size = 0;

    return list;
}

int list_push(t_List* list, void* data) {
    t_Node* temp_node = node_create(data);

    if (temp_node == NULL) {
        std::cout << "Failed malloc Node\n";
        return -1;
    }

    if (list->size == 0) {
        list->head = temp_node;
        list->tail = temp_node;
    } else {
        list->tail->next = temp_node;
        temp_node->previous = list->tail;
        list->tail = temp_node;
    }

    list->size++;

    return 0;
}

void* list_pop(t_List* list) {
    if (list->size == 0) {
        std::cout << "The list is empty!\n";
        return NULL;
    }

    void* data = list->tail->data;

    t_Node* new_tail = list->tail->previous;
    if (new_tail != NULL) {
        new_tail->next = NULL;
    }
    delete list->tail;

    list->tail = new_tail;
    list->size--;

    return data;
}

int list_add_first(t_List* list, void* data){
    if(list->size == 0){
        list_push(list, data);
        return 0;
    }

    t_Node* temp_node = node_create(data);
    if(temp_node == NULL){
        printf("Failed malloc Node\n");
        return -1;
    }

    t_Node* aux_node = list->head;
    list->head->previous = temp_node;
    temp_node->next = aux_node;
    list->head = temp_node;
    list->size++;

    return 0;
}

int list_remove_first(t_List* list){
    if(list->size == 0){
        printf("The list is empty!\n");
        return -1;
    }

    t_Node* temp_node = list->head->next;
    if(temp_node != NULL){
        temp_node->previous = NULL;
    }

    free(list->head);
    list->head = temp_node;
    list->size--;

    return 0;
}

// Return: 0 OK, -1 Couldn't find it
int list_remove(t_List* list, void* data){
    if(list->size == 0){
        printf("The list is empty!\n");
        return -1;
    }

    t_Node* aux_node = list->head;
    while(aux_node != NULL){
        if(aux_node->data == data){
            if(aux_node->previous != NULL){
                aux_node->previous->next = aux_node->next;
            }else{
                list->head = aux_node->next;
            }
            if(aux_node->next != NULL){
                aux_node->next->previous = aux_node->previous;
            }else{
                list->tail = aux_node->previous;
            }

            free(aux_node);

            list->size--;
            return 0;
        }
        aux_node = aux_node->next;
    }
    return -1;
}

void list_print(t_List* list) {
    if (list->size == 0) {
        std::cout << "The list is empty!\n";
        return;
    }

    std::cout << "List Size: " << list->size << std::endl;

    t_Node* temp_node = list->head;
    while (temp_node != NULL) {
        std::cout << "Node " << temp_node << std::endl;
        temp_node = temp_node->next;
    }
}

void list_erase(t_List* list) {
    while (list->size > 0) {
        delete list_pop(list);
    }

    delete list;
    list = NULL;
}

void* list_get(t_List* list, int index) {
    if (index >= list->size || index < 0) {
        return NULL;
    }

    t_Node* current_node = list->head;

    while (index > 0) {
        if (current_node == NULL) {
            return NULL;
        }
        current_node = current_node->next;
        index--;
    }

    return current_node->data;
}

int list_find(t_List* list, void* data) {
    int index_counter = 0;

    t_Node* temp_node = list->head;
    while (temp_node != NULL) {
        if (temp_node->data == data) {
            return index_counter;
        }
        temp_node = temp_node->next;
        index_counter++;
    }
    return -1;
}