# Sensitivity and Specificity Analysis for 3D Imaging Data 

[VTK surface data](http://www.cacr.caltech.edu/~slombey/asci/vtk/vtk_formats.simple.html) is constructed from point (or vertex) and cells (or surface polygons). It can accomodate data in both points and cells. 

To transfer the data held in cells to points, the [VTK API](http://www.vtk.org/doc/nightly/html/index.html) offers the [vtkCellDataToPointData class](http://www.vtk.org/doc/nightly/html/classvtkCellDataToPointData.html) to perform this conversion. 

## The code 

The code in this repositories transfers the cell data to point data. The method of transformation is based on averaging the data values of all cells using a particular point.

## Dependencies
Note that this program uses [VTK 7](http://www.vtk.org/doc/nightly/html/index.html). 

It is expected to work on other older versions of VTK, with minor changes to the ```->SetInputData()``` functions. These should be changed appropriately to ```->SetInput()```. 

## Usage 
The usage for the code once it's compiled is:
```
./the_program_name
		input_vtk_filename
		output_vtk_filename
```




## Author 
```
Dr. Rashed Karim 
Department of Biomedical Engineering 
King's College London 
```
