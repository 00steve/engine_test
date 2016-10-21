/*an implimentation of the load class that loads an
.bmp into memory. Where it goes from there is up to
whatever is using it. All it guarentees is that whatever
is calling it will get a 2d array of values back.
*/

#ifndef BMP_LOAD_H
#define BMP_LOAD_H
#include "../core/loader.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../core/string_util.h"
#include "../graphics/texture.h"


class load_bmp : public loader {
private:

    void custom_load(string file_name){

		#if DEBUG_LOAD
			cout << "load::bmp(" << file_name << ")\n";
		#endif

        //create a file stream
        ifstream file;
        char* header = new char();

        //attempt to open the file to read
        file.open(file_name.c_str());
        if(!file.good()) {
			cout << "file doesn't exist, bitch\n";
            failed();
            return;
        }

		#if DEBUG_LOAD
			cout << "Bitmap\n--------------------\n";
		#endif
		/*get the length of the file*/
		int fsize = file.tellg();
		file.seekg( 0, std::ios::end );
		fsize = file.tellg() - fsize;
		file.seekg(0);
		//cout << "File length: " << fsize << " bytes\n";


		/*get header file of the bmp image. It should have a
		first to chars that equal B and M (haha bowel movement).
		The length should be 54 (+ 1 for ifstream adding a \0 to the
		end of whatever is extracted out from the file. That is why
		it shows 55, but then has a -- to remove one from the lenth
		afterwards and other bumpkin jumpkin shit after that.
		*/
		file.get(header,55);
		if(!header || header[0] != 'B' || header[1] != 'M'){
			cout << " this bitmap is not a bm!\n";
			failed();
			return;
		}
		#if DEBUG_LOAD
		cout << "Type       : " << header[0] << header[1] << endl;
		#endif
		/*get the width and height of the image*/
		tex->setSize(int2(bytesToInt(&header[18]),bytesToInt(&header[22])));
		#if DEBUG_LOAD
		cout << "Dimensions : " << tex->getWidth() << "," << tex->getHeight() << endl;
		#endif
		/*figure out the bitdepth, which will determine how
		many bytes to use per color and other stuff like that,
		actually, fuck anyone who doesn't use 32 bit bitmaps,
		that is the only thing that I am going to support so if
		your bitmap is not 32 bit, fuck of, ya hoser!*/
		#if DEBUG_LOAD
		cout << "Bit depth  : " << bytesToShort(&header[28]) << endl;
		#endif
		if(bytesToShort(&header[28]) != 32){
			cout << "This isn't even 32 bit. Don't you even know that nobody even likes non-32 bit images?!\n";
			failed();
			return;
		}

        /*starting position of pixel data*/
		cout << "Pixel data : " << bytesToInt(&header[10]) << endl;
		cout << "PixelLength: " << tex->getWidth() * tex->getHeight() * 4 << endl;
		file.seekg(0);
        file.seekg(bytesToInt(&header[10]));
        int pixel_length = tex->getWidth() * tex->getHeight() * 4;
        char *pixels = new char[pixel_length]();
		file.get(pixels,pixel_length);

		int i=0, y=0, x=0;
		while(i < pixel_length){
			if(x == tex->getWidth()){
				++y;
				x=0;
			}
			++x;
			tex->pixel(x,y) = rgba(pixels[0],pixels[1],pixels[2],pixels[3]);
			i+=4;
		}

		file.close();

        cout << "finished loading " << file_name << "\n";
        succeeded();
		return;
    }

	texture* tex;

public:

    load_bmp(string file_name,texture* t) :
        loader(file_name),
        tex(t){
    }


	void integrateDependency(dependency* depends){


	}

    texture* get_image(){
        return tex;
    }

	void* getData(){ return tex; }

};

#endif // BMP_LOAD_H

