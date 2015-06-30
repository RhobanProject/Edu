/*
  Copyright (c) 2008 Florent D'halluin , Sylvain Calinon, 
  LASA Lab, EPFL, CH-1015 Lausanne, Switzerland, 
  http://www.calinon.ch, http://lasa.epfl.ch

  The program is free for non-commercial academic use. 
  Please acknowledge the authors in any academic publications that have 
  made use of this code or part of it. Please use this BibTex reference: 
 
  @article{Calinon07SMC,
  title="On Learning, Representing and Generalizing a Task in a Humanoid 
  Robot",
  author="S. Calinon and F. Guenter and A. Billard",
  journal="IEEE Transactions on Systems, Man and Cybernetics, Part B. 
  Special issue on robot learning by observation, demonstration and 
  imitation",
  year="2007",
  volume="37",
  number="2",
  pages="286--298"
  }
*/

#include "MathLib.h"
#include "gmr.h"

#define NBSTATES 5   // Number of states in the GMM 
#define NBSAMPLES 5  // Number of samples used to train the GMM
#define NBMOTORS 7   // Number of motors

int main(int argc, char *argv[]) 
{ 
  GaussianMixture g;

	if( argc != 2){
		std::cout << "Placez le nom du fichier, sans numero ni extension, en paramètre, merci." << std::endl;
		return -1;
	}

  ///////////////////////////////////////////////////////////////////////
  std::cout << "Load data from './indata/'..." << std::flush;
  Matrix Data[NBSAMPLES];

  char filename[256];
  for (unsigned int i = 0; i < NBSAMPLES; i++){
    sprintf(filename,"./indata/%s%.2d.txt",argv[1], i+1);
    Data[i] = g.loadDataFile(filename); 
  }
  std::cout << "ok" << std::endl;

  ///////////////////////////////////////////////////////////////////////

  Matrix speedData[NBSAMPLES];

	for (unsigned int i = 0; i < NBSAMPLES; i++){
		speedData[i] = g.speedCompute(Data[i]);
		  sprintf(filename,"./outdata/speed%.2d.txt",i+1);
		    g.saveDataFile(filename,speedData[i]);
	}

	Matrix speedPos[NBSAMPLES];
	for (unsigned int i = 0; i < NBSAMPLES; i++){
		speedPos[i] = g.speedPosition(Data[i], speedData[i]);
		sprintf(filename,"./outdata/speedPos%.2d.txt", i+1);
		g.saveDataFile(filename,speedPos[i]);
	}


  ///////////////////////////////////////////////////////////////////////
  std::cout << "Load speedPosition from './outdata/'..." << std::flush;
  Matrix rawData[NBSAMPLES];
  unsigned int nbData=0;
  for (unsigned int i = 0; i < NBSAMPLES; i++){
    sprintf(filename,"./outdata/speedPos%.2d.txt", i+1);
    rawData[i] = g.loadDataFile(filename); 
    nbData += rawData[i].RowSize(); 
  }
  nbData = (int)(nbData/NBSAMPLES);
  unsigned int nbVar = rawData[0].ColumnSize();
  std::cout << "ok" << std::endl;



  ///////////////////////////////////////////////////////////////////////
  std::cout << "Rescale the raw data and save the result to './outdata'..." 
	    << std::flush; 
  Matrix interpol, dataset;

  interpol.Resize(nbData,nbVar);
  for (unsigned int i = 0; i < NBSAMPLES; i++){
    g.HermitteSplineFit(rawData[i],nbData,interpol); 
    dataset = interpol.VCat(dataset);
    sprintf(filename,"./outdata/data%.2d_rescaled.txt",i+1);
    g.saveDataFile(filename,interpol);
  }
  std::cout << "ok" << std::endl;

  /////////////////////////////////////////////////////////////////////// 
  std::cout << "Learn the GMM model and save the result to './outdata'..." 
	    << std::flush; 
  g.initEM_TimeSplit(NBSTATES,dataset); // initialize the model
    std::cout << "TimeSplitInit" 	    << std::endl; 
  g.doEM(dataset); // performs EM
	  std::cout << "doEMdone" 	    << std::flush; 
  g.saveParams("./outdata/gmm.txt");
  std::cout << "ok" << std::endl;





  ///////////////////////////////////////////////////////////////////////
  std::cout << "Apply the GMR regression and save the result to './outdata'..." 
	    << std::flush; 
  Vector inC(NBMOTORS), outC(NBMOTORS);
  for (unsigned int i = 0; i < NBMOTORS; i++){
	 inC(i)= (float)(i+1); // Columns of the input data for regression (here, motors positions)
  }
  for(unsigned int i=7;i<14;i++) 
    outC(i - 7)=(float)(i+1); // Columns for output : remainings



  Matrix inData = rawData[0].GetColumnSpace(1,7);


		for (int i = 0; i < 250; i++){
			std::cout << " " << std::endl;
			for (int j = 0; j < 8; j++)
				std::cout << inData(i,j) << std::flush;
		}



  Matrix *outSigma;
  outSigma = new Matrix[nbData];
  Matrix outData = g.doRegression(inData,outSigma,inC,outC);
  g.saveRegressionResult("./outdata/gmr_Mu.txt","./outdata/gmr_Sigma.txt", 
			inData, outData, outSigma);
  std::cout << "ok" << std::endl;
//  std::cout << "You can now run 'plotall' in matlab (or type 'make plot' here) to display the GMM/GMR results." << std::endl;
  

  Vector posInit(7);

  for (int j = 0; j < 7; j++)
		//posInit(j) = (rawData[0](0,j+1) + rawData[1](0,j+1) + rawData[2](0,j+1)) / 3;
		posInit(j) = rawData[0](0,j+1);

  Matrix FinalPos(250,8);

  FinalPos = g.getPosition(outData, posInit);
  		sprintf(filename,"./outdata/data_Final.txt");
		g.saveDataFile(filename,FinalPos);

  return 0;
}
