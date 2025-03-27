#ifndef _IPC_IPC_TOPIC_FACTORY_HPP_
#define _IPC_IPC_TOPIC_FACTORY_HPP_

#include <functional>
#include <map>
#include <string>
#include <iostream>
#include <thread>

#include <assert.h>
#include <signal.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/shm.h>
#include <unistd.h>

class ipc_node;

struct topic
{
    const char* name;
    int segment_id;
    key_t segment_key;
    atomic_char* guard;
    char * shared_memory;
};

class ipc_node
{
    public:

    ipc_node() {}

    template<typename type>
    std::pair<topic, topic> create_topic(const char* name, std::function<void(ipc_node&, topic&, char* buffer, int size)> handler)
    {
        
        auto recv_segment_key = generate_recv_key(name);
        auto recv_segment_id = shmget(recv_segment_key, 1 + sizeof(type), IPC_CREAT | 0666);
        char* recv_shared_memory = (char*)shmat(recv_segment_id, NULL, 0);

        auto send_segment_key = generate_send_key(name);
        auto send_segment_id = shmget(send_segment_key, 1 + sizeof(type), IPC_CREAT | 0666);
        char* send_shared_memory = (char*)shmat(send_segment_id, NULL, 0);

        topic recv_topic = {
            .name = name,
            .segment_key = recv_segment_key,
            .segment_id = recv_segment_id,
            .shared_memory = recv_shared_memory,
        };

        topic send_topic = {
            .name = name,
            .segment_key = send_segment_key,
            .segment_id = send_segment_id,
            .shared_memory = send_shared_memory,
        };

        return std::make_pair(std::move(send_topic), std::move(recv_topic));
    }

    template<typename type>
    void send(topic& topic, type& message) {
        memcpy(topic.shared_memory + 1, &message, sizeof(type));
        notify(topic.guard);
    }

    template<typename type>
    void recv(topic& topic, type& message) {
        wait(topic.guard);
        memcpy(&message, topic.shared_memory + 1, , sizeof(type));
    }

    protected:

    void wait(atomic_char* guard) {
        while (atomic_load(guard) != 'w')
            ;
    }

    void notify(atomic_char* guard) {
        atomic_store(guard, 'n');
    }

    key_t generate_recv_key(const char* path) {
        return ftok(path, 'recv');
    }

    key_t generate_send_key(const char* path) {
        return ftok(path, 'send');
    }

    virtual ~ipc_node() {}
    private:
    
};

#endif