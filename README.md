# Sensitiity and Specificity Analysis for 3D Imaging Data 

## Sensitivity

The sensitivity of a test is defined as the proportion of people with disease who will have a positive result. If we apply a test to a hypothetical population (10 ppl have disease out of 100), and 8 of the 10 people with Disease A test positive, then the sensitivity of the test is 8/10 or 80%.

## Specificity

The specificity of a test is the proportion of people without the disease who will have a negative result.
Positive predictive:The positive predictive value (PPV) of a test is defined as the proportion of people with a positive test result who actually have the disease

The code in this repositories computes this on the entire image and also locally on specified regions.  

## Usage
Since specificity gives the proportion of negative results that have tested negative, it is important to tell the program what is the search space of the algorithm. This is sometimes not the entire image as some algorithms are restricted to a subset in the image. As most images have a higher proportion of background pixels (i.e. the negatives in our test), the specificity if computed on the entire image will give a value that is biased.

For a review on sensitivity and specificity see http://umanitoba.ca/faculties/medicine/units/pediatrics/sections/neonatology/media/DiagnosticTest1.pdf.

## Usage 
The usage for the first tool ```combinmask``` is:
```
./slice_dice -i1 <img1> -i2 <img2> -o <output dice txt> -txt <append text> --x --y --z <along which direction>
```
This program outputs the mean and standard deviation of dice in each slice along the specified X, Y, Z direction. 



## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
