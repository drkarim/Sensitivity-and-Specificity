# Sensitiity and Specificity Analysis for 3D Imaging Data 

```Sensitivity``` and ```specificity``` are statistical measures of the performance of a binary classification classification test, also known in statistics as Statistical classification function:

## Sensitivity

Sensitivity measures the proportion of positives that are correctly identified as such (i.e. the percentage of sick people who are correctly identified as having the condition).

## Specificity

```Specificity``` measures the proportion of negatives that are correctly identified as such (i.e., the percentage of healthy people who are correctly identified as not having the condition). It is also called the ```true negative rate```. 

## The code 

The code in this repositories computes this not only on the entire image, but also locally if the sub-regions are specified using xyz limits. 

## Dependencies
Note that this program uses [MIRTK - Medical Image Registration Library](https://github.com/BioMedIA/MIRTK), an old legacy version in [IRTK](https://wwwhomes.doc.ic.ac.uk/~rkarim/mediawiki/index.php?title=Sense_Dice_Code_Ver_2) exists but is not maintained anymore. 

The dependencies are not great on MIRTK and IRTK. It is simply for accessing pixel values in the image. I envisage that the code can be easily ported into [Insight Toolkit](https://github.com/InsightSoftwareConsortium/ITK) and other imaging libraries. 

## Important Notes
Since specificity gives the proportion of negative results that have tested negative, it is important to specifcy in the program the search space for computing specificity, taking into account background pixels (i.e. the negatives in our test). Consider the case when the specificity computed on a small binary mask inside a large image yields a biased high specificity value. 

For a review on sensitivity and specificity see http://umanitoba.ca/faculties/medicine/units/pediatrics/sections/neonatology/media/DiagnosticTest1.pdf.

## Usage 
The usage for the tool ```sensspec``` is. . :
```
./mirtk evaluate-senssepc <switches>:
		-t: Test-image
		-g: Ground truth
		-tL: Label in test img
		-gL: Label in ground truth
		-fp: Filename containing Local region bounds
		-sp: Image file for constraining the search space
		-o: Write Sens/spec  to file

		Defining local regions:
		=======================
		1) Sens/spec along a slice (x,y or z), switches:
			 -x <slice_number>
			 -y <slice_number>
			 -z <slice number> 

		2) Sens/spec inside a local region, switches:
			 -x1, -x2: Bounds in X direction where x1 < x2
			 -y1, -y2: Bounds in y
			 -z1, -z2: Bounds in z
```




## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
