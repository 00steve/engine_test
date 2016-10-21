#ifndef LBR_H
#define LBR_H

/*Lightweight Byte Record, or LapsterBator Recording, either one, fucker.

DATA FORMAT
1byte:number of columns
[
	1byte:number of bytes in each chunk of data
	1byte:data type code
	1byte:number of letters in column label
	bytes:letters of the column label
]
[
	xbyte:data for each column, in order, whatever number of bytes are specified
]
*/

#define DEBUG_LBR true
#define DEBUG_LBR_HEADER true
#define DATATYPE_BYTE 0
#define DATATYPE_LONG 1
#define DATATYPE_DOUBLE 2


#include <iostream>
#include <fstream>
#include <bitset>
#include "../../core/double3.h"
#include "../../core/ordered_list.h"


struct lbrc{ //lbr column structure
	string label;
	short chunkSize;
	short dataType;
	short dataSize;
	short arrayLength;
	char* data;

	int getByteOffset(int row){
		return row * chunkSize;
	}

	long getLong(int row,int column){
		return *reinterpret_cast<long*>(&data[row*chunkSize+column*dataSize]);
	}
	double getDouble(int row,int column){
		return *reinterpret_cast<double*>(&data[row*chunkSize+column*dataSize]);
	}

	template <typename t>
	list<t> getData(){return reinterpret_cast<t*>(data);}

};


class lbr{
private:

	ifstream 	log;
	string 		logName;
	long 		logByteSize;
	char		buffer[256];

	lbrc*		columns;
	short		columnCount;
	short 		currentColumn;
	int 		bytesPerRow;
	int			totalRows;


public:

	lbr() :
			columnCount(0),
			bytesPerRow(0),
			totalRows(0)
			{
	}

	~lbr(){
	}

	bool load(string fileName){

		logName = fileName;
		log.open(fileName.c_str(),ios::in|ios::binary|ios::ate);
		if(!log.is_open()) return false;
		log.seekg(0,ios::end);
		logByteSize = log.tellg();
		#if DEBUG_LBR
			cout << "Load LBR file : " << fileName << "\n---------------------\n\tSize : " << logByteSize << " bytes\n";
		#endif
		log.seekg(0,ios::beg);
		if(logByteSize < 1) return false;
		//get number of columns in log file
		log.read(buffer,1);
		columnCount = buffer[0];
		columns = new lbrc[columnCount]();
		#if DEBUG_LBR
			cout << "\tNumber of columns : " << columnCount << "\n";
		#endif

		int labelLength;
		currentColumn = 0;
		while(currentColumn < columnCount){
			log.read(buffer,3);
			columns[currentColumn].chunkSize =  (short)buffer[0];
			columns[currentColumn].dataType = (short)buffer[1];
				switch(columns[currentColumn].dataType){
				case DATATYPE_BYTE:
					columns[currentColumn].dataSize = 1;
					break;
				case DATATYPE_LONG:
					columns[currentColumn].dataSize = 4;
					break;
				case DATATYPE_DOUBLE:
					columns[currentColumn].dataSize = 8;
					break;
				}

			bytesPerRow += columns[currentColumn].chunkSize;
			labelLength = buffer[2];
			log.read(buffer,labelLength);
			columns[currentColumn].arrayLength = columns[currentColumn].chunkSize / columns[currentColumn].dataSize;
			columns[currentColumn].label = string(buffer,labelLength);
			#if DEBUG_LBR_HEADER
				cout << "\tColumn : " << columns[currentColumn].label << "\n";
				cout << "\tLabel length : " << labelLength << endl;
				for(int c=0;c<labelLength;c++){
					cout << "\t" << (int)columns[currentColumn].label[c];
				}
				switch(columns[currentColumn].dataType){
				case DATATYPE_BYTE:
					cout << "\t\tDatatype : byte [1]byte\n";
					break;
				case DATATYPE_LONG:
					cout << "\t\tDatatype : long [4]bytes\n";
					break;
				case DATATYPE_DOUBLE:
					cout << "\t\tDatatype : double [8]bytes\n";
					break;
				}
				cout << "\t\tColumn size : " << columns[currentColumn].chunkSize << " bytes\n";
				cout << "\t\tArray length : " << columns[currentColumn].arrayLength << "\n";

			#endif
			++currentColumn;
		}

		/*setup the data pointers to the correct size, based on the
		size of the file and how many bytes are left to read, divided by
		the number of bytes in each column.*/
		totalRows = (logByteSize-log.tellg()) / bytesPerRow;
		#if DEBUG_LBR
			cout << "\tRow count : " << totalRows << "\n";
			cout << "\tBytes per row : " << bytesPerRow << "\n";
		#endif

		/*loop through the columns and allocate the nesescary amount
		of bytes for the number of rows in the file x the number of
		bytes per row per column.*/
		currentColumn = 0;
		while(currentColumn < columnCount){
			columns[currentColumn].data = new char[columns[currentColumn].chunkSize*totalRows]();
			++currentColumn;
		}

		int currentRow = 0;
		while(!log.eof()){
			currentColumn = 0;
			while(currentColumn < columnCount){
				log.read(columns[currentColumn].data+columns[currentColumn].getByteOffset(currentRow),columns[currentColumn].chunkSize);
				#if DEBUG_LBR
					switch(columns[currentColumn].dataType){
					case DATATYPE_BYTE:
						//currentColumn
						break;
					case DATATYPE_LONG:
						//cout << columns[currentColumn].getLong(currentRow,0) << " | ";
						break;
					case DATATYPE_DOUBLE:
						for(int i=0;i<columns[currentColumn].arrayLength;i++){
							//cout << columns[currentColumn].getDouble(currentRow,i) << " ";
						}
						//cout << "| ";
						break;
					}
				#endif
				++currentColumn;
			}
			#if DEBUG_LBR
				//cout << endl;
			#endif
			++currentRow;
		}

		/*
		for(int i=0;i<totalRows;i++){
			for(int c=0;c<columns[1].arrayLength;c++){
				cout << columns[1].getDouble(i,c) << ",";
			}
			cout << endl;
		}*/


		log.close();

		return true;
	}


	int getRowCount(){ return totalRows; }

	template <typename t>
	t* getColumnData(string columnName,int index){
		cout << "get column " << columnName << "\n";
		t* data = NULL;
		currentColumn = 0;
		while(currentColumn < columnCount){
			if(columns[currentColumn].label != columnName){++currentColumn; continue;}
			cout << "found column " << columns[currentColumn].label << endl;
			data = new t[totalRows];
			switch(columns[currentColumn].dataType){
			case DATATYPE_LONG:
				for(int i=0;i<totalRows;i++){
					data[i] = (t)columns[currentColumn].getLong(i,index);
				}
				break;
			case DATATYPE_DOUBLE:
				cout << "is type DOUBLE\n";
				for(int i=0;i<totalRows;i++){
					data[i] = (t)columns[currentColumn].getDouble(i,index);
				}
				break;
			}
			return data;
			++currentColumn;
		}
		return NULL;
	}


};





#endif // LBR_H
