/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Copyright by The HDF Group.                                               *
 * Copyright by the Board of Trustees of the University of Illinois.         *
 * All rights reserved.                                                      *
 *                                                                           *
 * This file is part of HDF5.  The full HDF5 copyright notice, including     *
 * terms governing use, modification, and redistribution, is contained in    *
 * the COPYING file, which can be found at the root of the source code       *
 * distribution tree, or in https://support.hdfgroup.org/ftp/HDF5/releases.  *
 * If you do not have access to either file, you may request a copy from     *
 * help@hdfgroup.org.                                                        *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
/*
 *  This program shows how the select_hyperslab and select_elements
 *  functions are used to write selected data from memory to the file.
 *  Program takes 48 elements from the linear buffer and writes them into
 *  the matrix using 3x2 blocks, (4,3) stride and (2,4) count.
 *  Then four elements  of the matrix are overwritten with the new values and
 *  file is closed. Program reopens the file and reads and displays the result.
 */

#include <iostream>
#include <sstream>
#include <H5Cpp.h>
void write_4x4_array_to_hdf5();

const int DIM_COUNT_MAX = 100;
const int DIM_COUNT = 100;
int data[DIM_COUNT] =
    { 1, 2, 3
    };


using namespace H5;

void write_4x4_array_to_hdf5(int channel) {

    // Define the dataset dimensions
    const hsize_t CHUNK_SIZE = DIM_COUNT;
    hsize_t initial_dims[1] = {DIM_COUNT_MAX};
    hsize_t max_dims[1] = {H5S_UNLIMITED}; //, H5S_UNLIMITED};
    hsize_t chunk_dims[1] = {CHUNK_SIZE}; //, CHUNK_SIZE};

    hsize_t start[2];
    hsize_t newstride[2];
    hsize_t count[2];

    start[0]  =0;
    start[1]  = 0;
    newstride[0] = 1;
    newstride[1] = 1;
    count[0]  = DIM_COUNT;
    count[1]  = 1;

    // Define the file and dataset properties
    const H5std_string FILE_NAME("/users/o9p/cpp.h5");
    H5std_string DATASET_NAME("IntegerDataset");
    std::string chan("");// = new String();

    H5File file(FILE_NAME, channel ==0 ? H5F_ACC_TRUNC:H5F_ACC_RDWR);
    for (int chanNumber =0;chanNumber<96; chanNumber++)
    {
        std::ostringstream stream; // = new std::ostringstream();
        stream << chanNumber;
        chan = stream.str();
        DATASET_NAME="IntDataset";
        DATASET_NAME = DATASET_NAME.append(chan);

        // Create the dataspace
        DataSpace dataspace(1, initial_dims, max_dims);

        // Create the dataset creation property list and set the chunk size
        DSetCreatPropList dcpl;
        dcpl.setChunk(1, chunk_dims);

        // Create the dataset
        DataSet dataset = file.createDataSet(DATASET_NAME, PredType::NATIVE_INT, dataspace, dcpl);

        // Write the data to the dataset
        dataset.write(data, PredType::NATIVE_INT); //,DSetMemXferPropList::);
        dataset.close();
        dataspace.close();
    }
    file.close();
}

void append_4x4_array_to_hdf5(int channel) {
    // Define the file and dataset properties
    const H5std_string FILE_NAME("/users/o9p/cpp.h5");
    //const hsize_t CHUNK_SIZE = DIM_COUNT_MAX;
    std::string chan("");// = new String();
    std::ostringstream stream; // = new std::ostringstream();

    // Create the file
    H5File file(FILE_NAME, H5F_ACC_RDWR);
    for (int j=0;j<10000;j++)
    for (int channel = 0; channel < 96;channel++)
         {
//        H5File file(FILE_NAME, H5F_ACC_RDWR);
        std::string chan("");// = new String();
        std::ostringstream stream; // = new std::ostringstream();
        stream << channel;
        H5std_string DATASET_NAME("IntegerDataset");
        chan = stream.str();

        DATASET_NAME = DATASET_NAME.append(chan);
        DataSet dataset = file.openDataSet(DATASET_NAME); //, PredType::NATIVE_INT, dataspace, dcpl);

        hsize_t start[2];
        hsize_t newstride[2];
        hsize_t count[2];

        start[0]  =0;
        start[1]  = 0;
        newstride[0] = 1;
        newstride[1] = 1;
        count[0]  = DIM_COUNT;
        count[1]  = 1;
        IntType tp = dataset.getIntType();
        hsize_t dims_out[2];
        DataSpace dataspace = dataset.getSpace();
        int rank = dataspace.getSimpleExtentNdims();
        int ndims = dataspace.getSimpleExtentDims( dims_out, NULL);
        hsize_t      size[2] = {dims_out[0]+DIM_COUNT,1};
        dataset.extend(size);
        DataSpace myspace = dataset.getSpace();
        DataSpace dyspace = dataset.getSpace();

        myspace.selectHyperslab(H5S_SELECT_SET, count, start, newstride);
        start[0]  =dims_out[0];
        dyspace.selectHyperslab(H5S_SELECT_SET, count, start, newstride);
        dataset.write(data, PredType::NATIVE_INT, myspace, dyspace); //, file_space, H5P_DEFAULT);
//        dataset.write(data, PredType::NATIVE_INT, myspace, dyspace); //, file_space, H5P_DEFAULT);
        myspace.close();
        dyspace.close();
        dataspace.close();
        dataset.close();
    //    file.close();
    }
    file.close();
}


int main() {
    for (int i=0;i<DIM_COUNT;i++)data[i]=333+i;
    const H5std_string FILE_NAME("/users/o9p/cpp.h5");
    //H5File file(FILE_NAME, true ? H5F_ACC_TRUNC:H5F_ACC_RDWR);
   // FILE *fp = fopen("/users/o9p/cpp.bin", "w+");

   //  for (long i=0;i<10000;i++)
   //  {
   //      FILE *fp = fopen("/users/o9p/cpp.bin", "w+");
   //      for (long y=0;y<96;y++)
   //          fwrite(data, sizeof(int), DIM_COUNT, fp);
   //      fclose(fp);
   //  }
   // //fclose(fp);
   //  std::cout << "Successfully created extendible chunked integer raw dataset and wrote array to it! "<< DIM_COUNT << std::endl;
   // return 0;
    try {
//        for (int x=0;x<10000;x++)
        for (long i=0;i<1;i++)
        {
        write_4x4_array_to_hdf5(i);
        }
       for (int x=0;x<0;x++)
       {
            for (int i=0;i<1;i++)
            {
               append_4x4_array_to_hdf5(i);
            }
       }
    } catch (Exception error) {
        return -1;
    }
    std::cout << "Successfully created extendible chunked integer HDF5 dataset and wrote array to it! "<< DIM_COUNT << std::endl;
    return 0;
}

