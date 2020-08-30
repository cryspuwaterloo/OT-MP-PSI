#include <NTL/ZZ_p.h>
#include <NTL/ZZ.h>
#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <time.h>
#include <vector>
#include <omp.h>
#include "Recon.h"

using namespace std;
using namespace NTL;

size_t binCoeff(size_t n, size_t k) {

  if ((k==0)||(n==k)) {
    return 1;
  } else if (k>n/2) {
    return binCoeff(n, n-k);
  } else {
    return binCoeff(n-1, k-1) + binCoeff(n-1, k);
  }
}

class Combinations {
	public:
    Combinations(vector<int> elems, int n, int r)
        : chosenUsers{elems}
        , n{n}
        , r{r}
    {
      this->s = binCoeff(n, r);
    }

	size_t size() {
		return this->s;
	}

    bool next() {
        int positionToIncrement = r - 1;
        while (chosenUsers[positionToIncrement] == n - r + (positionToIncrement)) {
            --positionToIncrement;
        }
        if (positionToIncrement < 0) {
            return false;
        }
        ++chosenUsers[positionToIncrement];
        for (int i = positionToIncrement + 1; i < r; ++i) {
            chosenUsers[i] = chosenUsers[positionToIncrement] + (i - positionToIncrement);
        }
        return true;
    }
    std::vector<int> getElements() {
        return chosenUsers;
    }

	private:
    vector<int> chosenUsers;
    int n;
    int r;
    size_t s;
};

int incBinIndices(vector<int> &binIndices, int t, int binSize){
	int i = t;
	do{
		if(i == 0){
			return 0;
		}
		i = i - 1;
		binIndices[i] = (binIndices[i] + 1) % binSize;
	} while(binIndices[i] == 0);
	return 1;
}

//Main Logic: takes in m * max_bin_size "matrix" of Shares, outputs a 2D binary vector containing which elements reconstructed for each user
vector<vector<int>> recon_in_bin_x(vector<vector<Share>> shares, Context context, int m, int max_bin_size, int scheme, int* count){

	ZZ_p::init(ZZ(context.p));
	vector<vector<int>> toReturn(m, vector<int>(max_bin_size,0));
	//Initialize first combination (first t bins)
	vector<int> startingPoint(context.t);
	for(int i = 0 ; i < context.t ; i++){
		startingPoint[i] = i;
	} 

    //Store all combinations in memory
    std::vector<std::vector<int>> combArray(binCoeff(m, context.t));
    Combinations comb{startingPoint, m, context.t};
    size_t i = 0;
    do {
      combArray[i] = comb.getElements();
      i++;
    } while(comb.next());

    //For each combinations of users do recon on the users in chosen indicies
    ZZ_pContext mcontext;
    mcontext.save();
    #pragma omp parallel
    {
      mcontext.restore();
      
		#pragma omp for
		for (size_t runner = 0; runner < comb.size(); runner++) {
			int reconstructed = 0;
			vector<int> chosenUsers=combArray[runner];
			ZZ lagrange_coeffs[context.t];
			ZZ new_mod;
			if (scheme==1)
				new_mod=context.p;
			else
				new_mod=context.q;
				
			{ // scope change for modulus switch
				ZZ_pPush push;
				ZZ_p::init(new_mod); //intialize new modulus
				for(int i = 0; i<context.t ; i++){
					ZZ_p numerator = ZZ_p(1);
					ZZ_p denominator = ZZ_p(1);
					for(int j=0; j<context.t; j++){
						if(i != j){
							numerator *= conv<ZZ_p>(shares[chosenUsers[j]][0].id);
							denominator *= conv<ZZ_p>(shares[chosenUsers[j]][0].id - shares[chosenUsers[i]][0].id);
						}
					}
					lagrange_coeffs[i] = conv<ZZ>(numerator / denominator);
				}
			}

			vector<int> binIndices(max_bin_size);
			//Initialize the bin indices for this combination of users
			for(int i = 0 ; i < max_bin_size ; i++){
				binIndices[i] = 0;
			}
			do{
				//Do recon on chosen users bins (from chosenUsers) using the an element from each bin (from binIndices)
				vector<Share> toRecon(context.t);
				for(int i = 0 ; i < context.t ; i++ ){
					toRecon[i] = shares[chosenUsers[i]][binIndices[i]];
				}
				if (scheme==1){
					ZZ_p secret;
					secret = ZZ_p(0);

					for(int i = 0; i<context.t; i++){
						secret += conv<ZZ_p>(toRecon[i].SS) * conv<ZZ_p>(lagrange_coeffs[i]);
					}
					reconstructed=(secret == 0); 
				}else{
					ZZ_p secret;
					secret = ZZ_p(1);

					for(int i = 0; i<context.t; i++){
						secret *= NTL::power(conv<ZZ_p>(toRecon[i].SS),lagrange_coeffs[i]);
					}
					reconstructed=(secret == 1); 
				}
				if(reconstructed){ //If reconstructed, add to the list toReturn
					int alreadyFound = 0;
					#pragma omp critical
					{
						for(int j = 0; j < context.t ; j++){
						bool b;
						b = toReturn[chosenUsers[j]][binIndices[j]];
						
							if(b == 1){
								alreadyFound = 1;
							}

							toReturn[chosenUsers[j]][binIndices[j]] = 1;
						}
					}

					if(!alreadyFound){
					#pragma omp atomic
						(*count)++;
					}
				}

			} while(incBinIndices(binIndices,context.t,max_bin_size));

		}
    }
	return toReturn;
}