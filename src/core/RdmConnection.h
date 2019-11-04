#ifndef RDMCONNECTION_H
#define RDMCONNECTION_H
#include <rdma/fi_domain.h>
#include <rdma/fabric.h>
#include <rdma/fi_cm.h>
#include <rdma/fi_endpoint.h>
#include <rdma/fi_tagged.h>
#include <rdma/fi_rma.h>
#include <string.h>

#include <mutex>
#include <vector>
#include <map>
#include <unordered_map>

#include "HPNL/Connection.h"
#include "HPNL/BufMgr.h"

#include <jni.h>

class RdmConnection : public Connection {
  public:
    RdmConnection(fid_domain *, fi_info*, fid_av*, uint64_t, fid_cq*,
    		fid_ep*, fid_ep*, BufMgr*, BufMgr*, bool);
    ~RdmConnection();
    virtual int init(int, int, int, uint64_t, int, int) override;

    void init_addr();
    void get_addr(char**, size_t*, char**, size_t*);

    virtual int send(Chunk*) override;
    virtual int send(int, int) override;
    virtual int sendRequest(int, int) override;
    virtual int sendBuf(char*, int, int, int) override;
    virtual int sendBufWithRequest(char*, int, int, int) override;
    virtual int sendTo(int, int, uint64_t) override;
    virtual int sendBufTo(char*, int, int, int, uint64_t) override;
    virtual int read(int, int, uint64_t, uint64_t, uint64_t) override;
    virtual int read(int, uint64_t, uint64_t, int, uint64_t, uint64_t, uint64_t) override;

    virtual char* get_peer_name() override;
    char* get_local_name();
    int get_local_name_length();
    fid_cq* get_cq();
    virtual void reclaim_chunk(Chunk*) override;
    virtual int activate_chunk(Chunk*) override;
    virtual int activate_chunk(int) override;
    virtual uint64_t reg_rma_buffer(uint64_t, uint64_t, int) override;
    virtual void unreg_rma_buffer(int) override;
    std::vector<Chunk*> get_send_buffer();
    std::vector<Chunk*> get_recv_buffer();

    virtual void set_recv_callback(Callback*) override;
    virtual void set_send_callback(Callback*) override;
    virtual Callback* get_recv_callback() override;
    virtual Callback* get_send_callback() override;
    virtual void decode_peer_name(void*, char*) override;
    virtual char* decode_buf(void *buf) override;

    virtual uint64_t resolve_peer_name(char*);
    virtual void adjust_send_target(int);

    void set_id(long id_){
    	connect_id = id_;
    }
    long get_id() { return connect_id; }

    void set_local_name(char *local_name, size_t local_name_len, fi_addr_t local_provider_addr){
    	this->local_name = local_name;
    	this->local_name_len = local_name_len;
    	this->local_provider_addr = local_provider_addr;
    }

    jobject get_java_conn(){
       	return java_conn;
    }

	void set_java_conn(jobject java_conn_){
		java_conn = java_conn_;
	}

	jmethodID get_java_callback_methodID(){
		return java_callback_methodID;
	}

	void set_java_callback_methodID(jmethodID methodID){
		java_callback_methodID = methodID;
	}

	void set_accepted_connection(bool ac){
		accepted_connection = ac;
	}

	bool is_accepted_connection(){
		return accepted_connection;
	}
  private:
    fid_fabric *fabric;
    fi_info *info;
    fid_domain *domain;
    fid_ep *tx;
    fid_ep *rx;
    fid_av *av;
    fid_cq *conCq;
    fid_eq *conEq;
    
    uint64_t recv_tag;
    uint64_t send_tag;

    const char* ip;
    const char* port;
    char *local_name;
    size_t local_name_len = 64;
    fi_addr_t dest_provider_addr;
    fi_addr_t local_provider_addr;
    fi_addr_t recv_ctx_addr;
    fi_addr_t send_ctx_addr;
//    std::map<std::string, fi_addr_t> addr_map;
    BufMgr *rbuf_mgr;
    BufMgr *sbuf_mgr;
    std::vector<Chunk*> recv_buffers;
    std::vector<Chunk*> send_buffers;
    std::unordered_map<int, Chunk*> send_buffers_map;
    int ctx_num;

    Chunk **send_global_buffers_array;

    std::unordered_map<int, Chunk*> rdma_buffers_map;

    bool is_server;
    bool accepted_connection;

    Callback* recv_callback;
    Callback* send_callback;
    bool inited = false;

    size_t dest_port;
    char dest_addr[20];
    size_t src_port;
    char src_addr[20];

    long connect_id;

    std::mutex conMtx;

    jobject java_conn;
    jmethodID java_callback_methodID;
};

#endif
