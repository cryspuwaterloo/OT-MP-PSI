#ifndef __PSI_UTILS__
#define __PSI_UTILS__

#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>
#include "nlohmann/json.hpp"
#include <fstream>
#include <iostream>
#include <sstream>
#include <time.h>
#include <vector>
#include <gmp.h>
#include <libhcs.h> 
#include <string>
#include <stdlib.h>

using namespace std;
using namespace NTL;
using json = nlohmann::json;

std::string ZZ_to_str(NTL::ZZ zz);
NTL::ZZ str_to_ZZ(std::string str);
NTL::ZZ read_prime(int bitsize);
NTL::ZZ read_generator(int bitsize);
string pcs_to_str(pcs_public_key* pk);
ZZ_p hash_(ZZ x, ZZ p);
ZZ_p hash_XX(ZZ x, ZZ p);
void ZZ_to_mpz_t(mpz_t __out, ZZ __temp_ZZ);
void ZZ_p_to_mpz_t(mpz_t __out, ZZ_p& num);
void mpz_t_to_ZZ(ZZ& __out, mpz_t num);
void mpz_t_to_ZZ_p(ZZ_p& __out, mpz_t num);

pcs_public_key* str_to_pcs(string str);
void str_to_mpz_t(mpz_t toReturn, string str);
string mpz_t_to_str(mpz_t num);

class Share{
    public:
	NTL::ZZ id;
	NTL::ZZ bin;
	NTL::ZZ SS;
	NTL::ZZ SS_mac;
    Share(){}
    Share(NTL::ZZ id_in, NTL::ZZ bin_in, NTL::ZZ SS_in);
    Share(int id_in, int bin_in, NTL::ZZ p); //For generating random nonsense shares for padding
};


class Context{
    public:
    NTL::ZZ p;
    NTL::ZZ q;
    NTL::ZZ g;
    int t;
    Context(){}
    Context(int pp, int tt){
        p = NTL::ZZ(pp);
        q = NTL::ZZ((p-1)/2);
        t = tt;
    }
    
    Context(int pp, int gg, int tt){
        p = NTL::ZZ(pp);
        q = NTL::ZZ((p-1)/2);
        g = NTL::ZZ(gg);
        t = tt;
    }

    Context(NTL::ZZ pp, NTL::ZZ qq, NTL::ZZ gg, int tt){
        p = NTL::ZZ(pp);
        q = NTL::ZZ(qq);
        g = NTL::ZZ(gg);
        t = tt;
    }

    void write_to_file(std::string filename){
    }
};

class KeyholderContext{
    public:
    int t;
    NTL::ZZ key;
    NTL::ZZ* randoms;
    KeyholderContext(){}
    KeyholderContext(int __t, int __key, NTL::ZZ __rands[]);

    void initialize_context(NTL::ZZ q, int __t);

    void initialize_from_file(std::string filename);

    void write_to_file(std::string filename);
};

class Scheme1_Round1_receive{
    public:
    vector<ZZ> masked_coefficients_alpha;
    Scheme1_Round1_receive(){}
    Scheme1_Round1_receive(string str);
    string toString();
};

class Scheme1_Round1_send{
    public:
    ZZ h_x_alpha; 
    ZZ g_alpha;
    Scheme1_Round1_send(){}
    Scheme1_Round1_send(string str);
    string toString();
};

class Scheme1_Round2_send{
    public:
    pcs_public_key *pk;
    int id;
    int t;
    mpz_t mpz_secret;
    mpz_t* mpz_coefficients;
    Scheme1_Round2_send(int t, pcs_public_key *__pk, int __id);
    Scheme1_Round2_send(string str);
    string toString();
};

class Scheme1_Round2_receive{
    public:
    mpz_t mpz_secret;
    Scheme1_Round2_receive();
    Scheme1_Round2_receive(string str);
    string toString();
};

class Scheme0_send{
    public:
    ZZ h_x_alpha;
    int id;
    Scheme0_send(){}
    Scheme0_send(string str);
    string toString();
};

class Scheme0_receive{
    public:
    ZZ secret_share_alpha;
    Scheme0_receive(){}
    Scheme0_receive(string str);
    string toString();
};

#endif