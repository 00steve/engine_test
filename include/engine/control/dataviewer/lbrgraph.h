#ifndef LBR_GRAPH_H
#define LBR_GRAPH_H

#include "lbr.h"
#include "lbrdatastream.h"

#define LBR_DISPLAY_COMBINED 0
#define LBR_DISPLAY_SEPERATE 1



class lbrGraph : public drawable{
private:

	string displayType;
	string styleType;
	int displayCode;
    list<lbrDataStream*> streams;
	string xName;

	double3 offset;

	double2 drawSize;

public:

	lbrGraph(string name,lbr* data,varMap settings){
		offset = settings.getDouble3("offset",double3(0,0,0));


		displayType = settings.getString("display","combined");
		if(displayType == "combined"){
			displayCode = LBR_DISPLAY_COMBINED;
		} else if(displayType == "seperate"){
			displayCode = LBR_DISPLAY_SEPERATE;
		} else {
			displayCode = LBR_DISPLAY_COMBINED; //default to combined if user entered invalid setting
		}


		/*load the data that represents the x dimension*/
		int rows = data->getRowCount();
        double* xData = new double[rows];
        //double* yData = new double[rows];

		/*get column information needed for building the streams*/
		varMap yColumns = settings.getGroup("yAxis");
		int cols = yColumns.getCount();

		/*build the streams based on number of y axis columns being
		displayed and the way the streams should be displayed.*/
		switch(displayCode){
		case LBR_DISPLAY_COMBINED:
			streams.push(new lbrDataStream(rows,cols));
			break;
		case LBR_DISPLAY_SEPERATE:
			for(int i=0;i<cols;i++){
				streams.push(new lbrDataStream(rows,1));
			}
			break;
		}

		/*load any columns being represented by the y
		values in the graph*/
		list<string> yColumnNames = yColumns.childrenNames();
		string columnLabel;
		int dataIndex;
		/*for each ycolumn, get the datta and then build a stream*/
		for(int i=0;i<yColumnNames.getCount();i++){
			columnLabel = yColumns.getString(yColumnNames[i],yColumnNames[i]);
            dataIndex = ((int)yColumnNames[i][yColumnNames[i].length()-1])-48;
            cout << "data index (last char of column name) " << dataIndex << endl;
            if(dataIndex > -1 && dataIndex < 10) {
				yColumnNames[i] = yColumnNames[i].substr(0,yColumnNames[i].length()-1);
            } else {
				dataIndex = 0;
            }
			switch(displayCode){
			case LBR_DISPLAY_COMBINED:
				cout << "column " << i << " = " << yColumnNames[i] << " data index = " << dataIndex << endl;
				streams[0]->setYData(data->getColumnData<double>(yColumnNames[i],dataIndex),i);
				break;
			case LBR_DISPLAY_SEPERATE:
				streams[i]->setYData(data->getColumnData<double>(yColumnNames[i],dataIndex),0);
				break;
			}
		}


		/*build specific stream type based on the graphing that needs
		to be done. The stream holds data points and renders them.*/
		styleType = settings.getString("style","line");


		xName = settings.getString("xAxis","");
		if(xName != ""){
			xData = data->getColumnData<double>(xName,0);
		}else{
			for(int i=0;i<data->getRowCount();i++){
				xData[i] = i+1;
			}
		}

		/*populate data into the x column of any streams*/
		switch(displayCode){
		case LBR_DISPLAY_COMBINED:
			streams[0]->setXData(data->getColumnData<double>(xName,0));
			break;
		case LBR_DISPLAY_SEPERATE:
			for(int i=0;i<streams.getCount();i++){
				streams[i]->setXData(data->getColumnData<double>(xName,0));
			}
			break;
		}


		/*get the bounds of the graph*/
		drawSize = settings.getDouble2("bounds",double2(10,10));
		fitToBounds();
	}

	void fitToBounds(){
		switch(displayCode){
		case LBR_DISPLAY_COMBINED:
			for(int i=0;i<streams.getCount();i++){
				streams[i]->fitToBounds(drawSize);
			}
			break;
		case LBR_DISPLAY_SEPERATE:
			double mult = streams.getCount()/ drawSize.y;
			for(int i=0;i<streams.getCount();i++){
				streams[i]->fitToBounds(drawSize*double2(1,mult));
			}
			break;
		}
	}

	void setDrawSize(double2 drawSize){
		this->drawSize = drawSize;
		fitToBounds();
	}


	void draw(){
		glPushMatrix();
			glTranslated(offset.x,offset.y,offset.z);
			for(int i=0;i<streams.getCount();i++){
				streams[i]->draw();
			}
		glPopMatrix();
	}


};



#endif // LBR_GRAPH_H
