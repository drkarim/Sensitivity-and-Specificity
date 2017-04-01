#include "mirtk/Common.h"
#include "mirtk/Options.h"
#include "mirtk/IOConfig.h"
#include "mirtk/GenericImage.h"

using namespace mirtk;

Array<double> i_ctr; 


void ReadRegionParamFile(char* fn, Array<int*>& params)
{
	ifstream* istream; 
	istream = new ifstream();
	istream->open(fn); 
	string line_in_file; 
	char* line;
	char* tok; 
	int k=0, line_num=0;
	int* bounds_xyz;

	if (istream->is_open())
	{
		while (!istream->eof())
		{
			getline(*istream, line_in_file);  
			line = new char[line_in_file.size()]; 
			if (line_in_file.size() > 3) {
				strcpy(line, line_in_file.c_str()); 

				tok = strtok(line, "\t"); 
				k = 0;
				bounds_xyz = new int[6];			
				while (tok != NULL)
				{
					bounds_xyz[k++] = atoi(tok); 
					tok = strtok(NULL, "\t");	
				}
				line_num++;
				params.push_back(bounds_xyz);
			}
		}
	}
	istream->close(); 
}

// re-orders the region params ensuring that x1<x2 and y1<y2 and z1<z2
void FixRegionParams(Array<int*>& params)
{
	double temp; 
	for (int i=0;i<params.size();i++)
	{
		// ensures that x1<x2 and y1<y2 and z1<z2
		for (int t=0;t<3;t++)
		{
			if (params[i][t+3] < params[i][t]) {
				temp = params[i][t];
				params[i][t] = params[i][t+3]; 
				params[i][t+3]=temp;
			}
			else if (params[i][t+3] == params[i][t])
			{
				params[i][t+3]++;			// if x1=x2 then must be input as x1, x2+1
			}
		}

		
	}
}

// Test image: input3 
// Ground truth: input4
// search space constraint: input5 (for example atrial wall)
void ReportDice(GreyImage* input3, GreyImage* input4, int _f_image1, int _f_image2, char* output=NULL,GreyImage* input5=NULL)
{
	  GreyPixel *ptr1; 
	  GreyPixel *ptr2;
	  GreyPixel *ptr3; 
		
	  
	double x_and_y=0, x=0, y=0, n=0, x_or_y=0, d_c, o_p1, o_p2; 
	double false_pos=0, false_neg=0, true_pos=0, true_neg=0,spec=0, sens=0, pos_pred=0, over_seg_meas=0, negs;
	bool isConstrained = false;
	

	   ptr1 = input3->GetPointerToVoxels();
       ptr2 = input4->GetPointerToVoxels();

	   if (input5 != NULL) 
	   {
		   cout << "got here" << endl;
		   ptr3 = input5->GetPointerToVoxels();			// search space constraint 
		   isConstrained = true; 
		    
	   }
	   //ptr3 = input3.GetPointerToVoxels();

	   cout << "number of voxels in Ground truth: " << input4->GetNumberOfVoxels() << endl;
	   cout << "number of voxels in test: " << input3->GetNumberOfVoxels() << endl;
	   if (isConstrained) cout << "number of voxels in constrained space: " << input5->GetNumberOfVoxels() << endl;
	   //cout << "number of voxels " << input3.GetNumberOfVoxels() << endl;

	   if (input3->GetNumberOfVoxels()-input4->GetNumberOfVoxels() == 0)

	   {
			for (int i=0;i<input3->GetNumberOfVoxels();i++)
		   {
			   if (*ptr1 == _f_image1 && *ptr2 == _f_image2) { 
				   if (isConstrained)
				   {
						if (*ptr3 > 0)
							x_and_y++;
				   }
				   else
				   {
						x_and_y++; 
				   }
			   }
			   if (*ptr1 == _f_image1)			// test image
			   {
				   if (isConstrained){
					   if (*ptr3 > 0)
						   x++;
				   }
				   else
					   x++; 
			   }
			   if (*ptr2 == _f_image2)			// ground truth
			   {
				   if (isConstrained){
					   if (*ptr3 > 0)
						   y++;
				   }
				   else
					   y++; 
				}
			   if (*ptr1 ==  _f_image1 || *ptr2 == _f_image2)
			   {
				   if (isConstrained){
					   if (*ptr3 > 0)
						   x_or_y++;
				   }
				   else
					   x_or_y++; 
			   }
			   if (*ptr1 == _f_image1 && *ptr2 == 0) 
			   {
				   if (isConstrained){
					   if (*ptr3 > 0)
						   false_pos++;
				   }
				   else
					   false_pos++; 
			   }
			   if (*ptr1 == 0 && *ptr2 == _f_image2)
			   {
				  if (isConstrained){
					   if (*ptr3 > 0)
						   false_neg++;
				   }
				   else
					   false_neg++;
			   }
			   if (*ptr1 == 0 && *ptr2 == 0)
			   {
					if (isConstrained){
					   if (*ptr3 > 0)
						   true_neg++;
				   }
				   else
					   true_neg++;
			   }
			   if (*ptr2 == 0)
			   {
					if (isConstrained){
					   if (*ptr3 > 0)
						   negs++;
				   }
				   else
					   negs++;
			   }
			   ptr1++; ptr2++;  n++; 
			   if (isConstrained) ptr3++;
		   }
			
			
			d_c = (2*((double)x_and_y))/((double)x+(double)y); 
			o_p1 = ((double)x_and_y/(double)x)*100;
			o_p2= ((double)x_and_y/(double)y)*100;
			sens = (x_and_y/(x_and_y+false_neg))*100; 
			spec = (true_neg/(true_neg+false_pos))*100; 
			pos_pred = (x_and_y/(x_and_y + false_pos))*100; 
			over_seg_meas = ((x_and_y+false_pos)/(x_and_y+false_neg))*100;

			if (x+y == 0) d_c = -1; 
			if (x == 0) o_p1 = -1; 
			if (y == 0) o_p2 = -1;

			if (output==NULL) { 
				cout << "\nDice co-efficients:\n=====================\n" << "Total voxels = " << n << "\nTest and ground truth overlap voxels = " << x_and_y << 
					"\nGround truth foreground voxels = " << y << "\nTest foreground voxels = " << x << endl; 
				cout << "Dice co-efficient = " << 10*d_c << endl; 
				cout << "Overlap as a percentage of ground truth voxels = " << o_p2 << endl; 
				cout << "Overlap as a percentage of test voxels = " << o_p1 << endl;
				cout << "\n\n==================================\n"; 
				double o_m = (double)x_and_y/(double)x_or_y; 
				cout << "Volumetric overlap metric (i.e. a_and_b / a_or_b)\nTest and GT = " << x_and_y << "\nTest and GT = "<< x_or_y << "\nOverlap metric = " << o_m << endl;
				cout << "Sensitivity and Specificity tests:\n" << "1. Sensitivity = " << sens << "\n2. Specificity = " << spec << "\n3. Positive"  
					" predicative value = " << pos_pred << endl;
cout << "4. Over-segmentation measure = " << over_seg_meas << endl;
			}
			else
			{
				ofstream fileIO; 
				fileIO.open(output, ios::app); 
				if (y > 0 && negs > 0)			// if there are no positves or no negatives in the ground truth, the TP and TN are both 0 and sens specs makes no sense
					fileIO << sens << "\t" << spec << endl;
				fileIO.close(); 
				cout << "Dice, pixel_in_foregrd_in_test, sensitivity and specifity written to file as\nDice\tSpecitifty\tSensitivity\tPositive predictive" << endl; 
			}
			

	   }
	   else
	   {
		   cout << "images with unequal dimensions, cannot compute the dice, must be equal dimensions" << endl;
	   }
}

int main(int argc, char **argv)
{
		
		int _f_image1=1, _f_image2=1;
		Array<int*> pms;

		// variables for sensitivity and specificty 
		
       char *filename1, *filename2, *output=NULL, *filename3, *filename4;
     
	   bool isWriteToFile = false;
	   int _fixX=-1, _fixY=-1, _fixZ=-1, _x1=-1, _y1=-1, _z1=-1,_x2=-1, _y2=-1, _z2=-1; 
	   int maxX, maxY, maxZ, maxT;

	   // required and optional arguments
		bool rArg1 = false, rArg2 = false, rArg3 = false, rArg4=false, rArg5=false; 
	   if (argc >= 1)
		{
			for (int i = 1; i < argc; i++) {
				if (i + 1 != argc) {
					if (string(argv[i]) == "-t") {			// test image
						filename1 = argv[i+1];			
						rArg1 = true;

					} else if (string(argv[i]) == "-g") {	// ground truth 
						filename2 = argv[i+1]; 
						rArg2 =  true; 
					} else if (string(argv[i]) == "-gL") {
						_f_image1 = atoi(argv[i+1]); 						
					} else if (string(argv[i]) == "-tL") {
						_f_image2 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-x") {
						_fixX = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-y") {
						_fixY = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-z") {
						_fixZ = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-x1") {
						_x1 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-y1") {
						_y1 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-z1") {
						_z1 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-x2") {
						_x2 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-y2") {
						_y2 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-z2") {
						_z2 = atoi(argv[i+1]); 
					} else if (string(argv[i]) == "-o") {
						output = argv[i+1];  
						isWriteToFile = true;
					} else if (string(argv[i]) == "-fp") { 
						filename3 = argv[i+1];
						rArg4 = true; 
					} else if (string(argv[i]) == "-sp") {
						filename4 = argv[i+1];
						rArg5 = true; 
					}
				}
			}
	   }
	
	   if ((!rArg1 && !rArg2) || argc <= 1) { 
		   cerr << "\nReports Sensitivity and specificty\n\nUsage: senspec <switches>:"
			   "\n\t\t-t: Test-image\n\t\t-g: Ground truth\n\t\t-tL: Label in test img\n\t\t-gL: Label in ground truth\n\t\t-fp: Filename containing Local region bounds"
			   "\n\t\t-sp: Image file for constraining the search space\n\t\t-o: Write Sens/spec  to file" 
			   "\n\n\t\tDefining local regions:\n\t\t=======================\n\t\t"
			   "1) Sens/spec along a slice (x,y or z), switches:\n\t\t\t -x <slice_number>\n\t\t\t -y <slice_number>\n\t\t\t -z <slice number> "
			   "\n\n\t\t2) Sens/spec inside a local region, switches:\n\t\t\t -x1, -x2: Bounds in X direction where x1 < x2\n\t\t\t -y1, -y2: Bounds in y\n\t\t\t -z1, -z2: Bounds in z"
			   "\n\n";
	
		exit(1); 
	   }
      

       // read imaged
       GreyImage input1;
       GreyImage input2;
	   GreyImage input3;
	   GreyImage input4;
	   GreyImage input5; 
	   GreyImage input6;


       input1.Read(filename1);
       input2.Read(filename2);			// ground truth
	   //input3.Read(filename3); 
	   
	   if (input1.GetX() != input2.GetX() || input1.GetY() != input2.GetY() || input1.GetZ() != input2.GetZ()) {
		cerr << "Image sizes dont match! Cant proceeed" << endl;
		exit(1);
	   }
	   else 
	   {
			maxX = input1.GetX(); maxY = input1.GetY(); maxZ = input1.GetZ(); maxT = input1.GetT(); 
	   }

	   if (rArg5) 
	   {
		   input5.Read(filename4);			// reading the search space for example atrial wall 
	   }

	   // if fixing x,y or z
	   if (!rArg4) {		// if parameters are declared through command-line and not through file 
		   if (_fixX > 0 || _fixY > 0 || _fixZ > 0) 
		   {
			   if (_fixX > 0)
			   {
					input3 = input1.GetRegion(_fixX, 0, 0, 0,_fixX+1, maxY, maxZ, maxT); 
					input4 = input2.GetRegion(_fixX, 0, 0, 0,_fixX+1, maxY, maxZ, maxT); 
					if (rArg5) input6 = input5.GetRegion(_fixX, 0, 0, 0,_fixX+1, maxY, maxZ, maxT); 
			   }
			   else if (_fixY > 0)
			   {
					input3 = input1.GetRegion(0, _fixY, 0, 0, maxX, _fixY+1, maxZ, maxT); 
					input4 = input2.GetRegion(0, _fixY, 0, 0, maxX, _fixY+1, maxZ, maxT); 
					if (rArg5) input6 = input5.GetRegion(0, _fixY, 0, 0, maxX, _fixY+1, maxZ, maxT); 
			   }
			   else if (_fixZ > 0)
			   {
				   input3 = input1.GetRegion(0, 0, _fixZ, 0, maxX, maxY, _fixZ+1, maxT); 
				   input4 = input2.GetRegion(0, 0, _fixZ, 0, maxX, maxY, _fixZ+1, maxT);		// ground truth 
				   if (rArg5) input6 = input5.GetRegion(0, 0, _fixZ, 0, maxX, maxY, _fixZ+1, maxT);
			   }
		   }
		   else {
				if (_x1 < 0 || _x2 < 0 || _y1 < 0 || _y2 < 0 || _z1 < 0 || _z2 < 0)
				{
					cerr << "Neither -x,-y,-z nor all of -x1,-x2,-y1,-y2,-z1,-z2 switches specified. Must specify region using either" << endl;
					exit(1); 
				}
				else if (_x1 > _x2 && _y1 > _y2 && _z1 > _z2)
				{
					cerr << "x1 must be less than x2 and so on for y and z" << endl;
					exit(1);
				}
				else
				{
					input3 = input1.GetRegion(_x1, _y1, _z1, 0, _x2, _y2, _z2, maxT); 
					input4 = input2.GetRegion(_x1, _y1, _z1, 0, _x2, _y2, _z2, maxT);			// groud truth 
					if (rArg5) input6 = input5.GetRegion(_x1, _y1, _z1, 0, _x2, _y2, _z2, maxT);
				}
		   }
		   if (rArg5) 
			ReportDice(&input3, &input4, _f_image1, _f_image2, output, &input6); 
		   else 
			ReportDice(&input3, &input4, _f_image1, _f_image2, output); 

	   } else {	
			// parameters declared through file
			ReadRegionParamFile(filename3, pms);
			FixRegionParams(pms);
			for (int i=0;i<pms.size();i++)
			{
				cout << "Dice for region: (x1,y1,z1) = (" << pms[i][0] << "," << pms[i][1] << "," << pms[i][2] <<")"
					", (x2,y2,z2) = (" << pms[i][3] << "," << pms[i][4] << "," << pms[i][5] <<")" << endl;
				input3 = input1.GetRegion(pms[i][0], pms[i][1], pms[i][2], 0, pms[i][3], pms[i][4], pms[i][5], maxT); 
				input4 = input2.GetRegion(pms[i][0], pms[i][1], pms[i][2], 0, pms[i][3], pms[i][4], pms[i][5], maxT); 
				if (rArg5) input6 = input5.GetRegion(pms[i][0], pms[i][1], pms[i][2], 0, pms[i][3], pms[i][4], pms[i][5], maxT);
				
				if (rArg5) 
					ReportDice(&input3, &input4, _f_image1, _f_image2, output, &input6); 
				   else 
					ReportDice(&input3, &input4, _f_image1, _f_image2, output); 
				
			}
	   }


   
}
