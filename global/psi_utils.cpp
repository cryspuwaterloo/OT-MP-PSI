#include "psi_utils.h"
#include <typeinfo>
using namespace std;
using namespace NTL;
using json = nlohmann::json;

ZZ_p hash_(ZZ x, ZZ p){
	ZZ_p::init(p);
	hash<std::string> ptr_hash;
	return ZZ_p(ptr_hash(ZZ_to_str(x)));
}

ZZ_p hash_XX(ZZ x, ZZ p){
	ZZ_p::init(p);
	hash<std::string> ptr_hash;
	ZZ_p __temp__ = ZZ_p(ptr_hash(ZZ_to_str(x)));
    return __temp__ * __temp__;
}

void ZZ_to_mpz_t(mpz_t __out, ZZ __temp_ZZ){
	std::stringstream ssa;
	ssa << __temp_ZZ;
	mpz_set_str( __out, ssa.str().c_str(), 10);
}

void ZZ_p_to_mpz_t(mpz_t __out, ZZ_p& num){
	ZZ __temp_ZZ;
	__temp_ZZ = rep(num);
	std::stringstream ssa;
	ssa << __temp_ZZ;
	mpz_set_str( __out, ssa.str().c_str(), 10);
}

void mpz_t_to_ZZ(ZZ& __out, mpz_t num){
	std::stringstream __ssa;
	__ssa << mpz_get_str(NULL, 10, num);
	__ssa >> __out;
}

ZZ read_prime(int bitsize){
    ifstream prime_file("../client/primes.json");
    json j;
    prime_file >> j;
    prime_file.close();
    string key = to_string(bitsize);
    string prime_str;
    if (j.find(key)==j.end()) {
        cout << "No prime for chosen bitsize, working with bitsize 1024" << endl;
        prime_str = j["1024"];
    } else {
        prime_str = j[key];
    }
    return conv<ZZ>(prime_str.c_str());
}

ZZ read_generator(int bitsize){
    ifstream prime_file("../client/generators.json");
    json j;
    prime_file >> j;
    prime_file.close();
    string key = to_string(bitsize);
    string prime_str;
    if (j.find(key)==j.end()) {
        cout << "No generator for chosen prime bitsize, working with bitsize 1024" << endl;
        prime_str = j["1024"];
    } else {
        prime_str = j[key];
    }
    return conv<ZZ>(prime_str.c_str());
}

Share::Share(ZZ id_in, ZZ bin_in, ZZ SS_in){
	id = id_in;
	bin = bin_in;
	SS = SS_in;
}

Share::Share(int id_in, int bin_in, ZZ p){
	id = ZZ(id_in);
	bin = ZZ(bin_in);
	ZZ_p::init(p);
	SS = rep(random_ZZ_p());
}

string ZZ_to_str(ZZ zz){
    std::stringstream ssa;
    ssa << zz;
    return ssa.str();
}

ZZ str_to_ZZ(string str){
    std::stringstream ssa;
    ssa << str;
    ZZ zz;
    ssa >> zz;
    return zz;
}

Scheme1_Round2_send::Scheme1_Round2_send(int __t, pcs_public_key *__pk, int __id){
    pk=__pk;
    id=__id;
    t=__t;
    mpz_init(mpz_secret);
    mpz_coefficients = (mpz_t *) malloc((t-1) * sizeof(mpz_t));
    for (int i=0;i<t-1;i++){
        mpz_init(mpz_coefficients[i]);
    }
}

Scheme1_Round2_receive::Scheme1_Round2_receive(){
    mpz_init(mpz_secret);
}

string pcs_to_str(pcs_public_key* pk)
{
    char *temp = pcs_export_public_key(pk);
    string toReturn(temp);
    return toReturn;
}

pcs_public_key* str_to_pcs(string str)
{
    pcs_public_key* toReturn=pcs_init_public_key();
    const char *temp = str.c_str();
    pcs_import_public_key(toReturn, temp);
    return toReturn;

}

void str_to_mpz_t(mpz_t toReturn, string str)
{
    mpz_set_str( toReturn, str.c_str(), 10);
}

string mpz_t_to_str(mpz_t num)
{
    return mpz_get_str(NULL, 10, num);
}

KeyholderContext::KeyholderContext(int __t, int __key, NTL::ZZ __rands[]){
    t = __t;
    key = NTL::ZZ(__key);
    randoms = __rands;
}

void KeyholderContext::initialize_context(NTL::ZZ q, int __t){
    t = __t;
    NTL::ZZ_p::init(q);
    key = NTL::rep(NTL::random_ZZ_p());
    randoms = new NTL::ZZ[t];
    for (int i=0;i<t-1;i++){
        randoms[i] = NTL::rep(NTL::random_ZZ_p());
    }
}

void KeyholderContext::initialize_from_file(std::string filename){
    std::ifstream inputFile(filename);
    json jsonFile;
    inputFile >> jsonFile;
    if(!inputFile.good())
    {
        cout<< "Could not open:"<<filename<<endl;
    }
    inputFile.close();
    t = jsonFile["t"] ;
    key = ZZ(INIT_VAL, jsonFile["key"].get<string>().c_str());
    randoms = new NTL::ZZ[t-1];
    for(int i = 0 ; i < t-1 ; i++)
    {
        randoms[i] = ZZ(INIT_VAL, jsonFile["randoms"][i].get<string>().c_str());
    }
}

void KeyholderContext::write_to_file(std::string filename){
    json jsonFile;
    ofstream outputFile(filename);
    jsonFile["t"] = t;
    jsonFile["key"] = ZZ_to_str(key);
    for(int i = 0 ; i < t ; i++)
    {
        jsonFile["randoms"][i] = ZZ_to_str(randoms[i]);
    }
    outputFile << jsonFile;
    outputFile.close();
 }

string Scheme1_Round1_receive::toString()
{
    string delim = "@";
    string toReturn = "";
    int size = masked_coefficients_alpha.size();
    toReturn += to_string(size);
    toReturn += delim;
    for(int i = 0 ; i<size ; i++)
    {
        toReturn += ZZ_to_str(masked_coefficients_alpha[i]);
        toReturn += delim;
    }
    return toReturn;
}

Scheme1_Round1_receive::Scheme1_Round1_receive(string str)
{
    stringstream ss(str);
    string token;
    char delim = '@';

    std::getline(ss, token, delim);
    int size = stoi(token);
    for(int i = 0 ; i<size ; i++)
    {
        std::getline(ss, token, delim); 
        masked_coefficients_alpha.push_back(str_to_ZZ(token));
    }
}

string Scheme1_Round1_send::toString()
{
    string delim = "@";
    string toReturn = "";
    toReturn += ZZ_to_str(h_x_alpha);
    toReturn += delim;
    toReturn += ZZ_to_str(g_alpha);
    return toReturn;
}

Scheme1_Round1_send::Scheme1_Round1_send(string str)
{
    stringstream ss(str);
    string token;
    char delim = '@';
    std::getline(ss, token, delim);
    h_x_alpha = str_to_ZZ(token);
    std::getline(ss, token, delim);
    g_alpha = str_to_ZZ(token);
}

string Scheme1_Round2_send::toString()
{
    string delim = "@";
    string toReturn = "";

    toReturn += pcs_to_str(pk);
    toReturn += delim;
    toReturn += to_string(id);
    toReturn += delim;
    toReturn += to_string(t);
    toReturn += delim;

    for(int i = 0 ; i < t-1 ; i++)
    {
        toReturn += mpz_t_to_str(mpz_coefficients[i]);
        toReturn += delim;
    }

    toReturn += mpz_t_to_str(mpz_secret);
    return toReturn;
}

Scheme1_Round2_send::Scheme1_Round2_send(string str)
{
    stringstream ss(str);
    string token;
    char delim = '@';
    std::getline(ss, token, delim);
    pk = str_to_pcs(token);

    std::getline(ss, token, delim);
    id = stoi(token);
    std::getline(ss, token, delim);
    t = stoi(token);

    mpz_coefficients = (mpz_t *) malloc((t-1) * sizeof(mpz_t));
    for(int i = 0 ; i<t-1 ; i++)
    {
        std::getline(ss, token, delim); 
        mpz_init(mpz_coefficients[i]);
        str_to_mpz_t(mpz_coefficients[i], token);
    }
    
    std::getline(ss, token, delim);
    mpz_init(mpz_secret);
    str_to_mpz_t(mpz_secret,token);
}

string Scheme1_Round2_receive::toString()
{
    string toReturn = mpz_t_to_str(mpz_secret);
    return toReturn;
}

Scheme1_Round2_receive::Scheme1_Round2_receive(string str)
{
    mpz_init(mpz_secret);
    str_to_mpz_t(mpz_secret,str);
}

string Scheme0_send::toString()
{
    string delim = "@";
    string toReturn = "";

    toReturn += ZZ_to_str(h_x_alpha);
    toReturn += delim;

    toReturn += to_string(id);

    return toReturn;
}

Scheme0_send::Scheme0_send(string str)
{
    stringstream ss(str);
    string token;
    char delim = '@';

    std::getline(ss, token, delim);
    h_x_alpha = str_to_ZZ(token);

    std::getline(ss, token, delim);
    id = stoi(token);
}

string Scheme0_receive::toString()
{
    string toReturn = ZZ_to_str(secret_share_alpha);
    return toReturn;
}

Scheme0_receive::Scheme0_receive(string str)
{
    secret_share_alpha = str_to_ZZ(str);
}