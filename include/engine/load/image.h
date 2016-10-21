
/*an implimentation of the load class that loads an
.bmp into memory. Where it goes from there is up to
whatever is using it. All it guarentees is that whatever
is calling it will get a 2d array of values back.
*/

#ifndef IMAGE_LOAD_H
#define IMAGE_LOAD_H
#include "../core/loader.h"
#include <string>
#include "../core/string_util.h"
#include "../graphics/texture.h"
#include "../graphics/FreeImage.h"



#define DEBUG_IMAGE_LOAD true


class load_image : public loader {
private:

    void custom_load(string fileName){
		#if DEBUG_LOAD
		cout << "load::img(" << fileName << ")\n";
		#endif
		string ext;
		getExt(fileName,ext);
		FREE_IMAGE_FORMAT extType;
		//try to load the image into the program
		ext = stringToLower(ext);
		if(ext == "png") {
			extType = FIF_PNG;
		} else if(ext == "bmp"){
			extType = FIF_BMP;
		} else if(ext == "jpg"){
			extType = FIF_JPEG;
		} else {
			failed();
			return;
		}
		FIBITMAP *bitmap = FreeImage_Load(extType, fileName.c_str(), 0);
		//make sure that the image was loaded successfully
		if (bitmap) {
			// bitmap successfully loaded!
			int w = FreeImage_GetWidth(bitmap);
			int h = FreeImage_GetHeight(bitmap);
			#if DEBUG_IMAGE_LOAD
				cout << "width: " << w << endl;
				cout << "height: " << h << endl;
			#endif
			GLuint id;
			GLubyte* textura = new GLubyte[4*w*h];
			//FreeImage loads in BGR format, so you need to swap some bytes(Or use GL_BGR).
			int bpp = FreeImage_GetBPP(bitmap);
			if(bpp == 24){
				#if DEBUG_IMAGE_LOAD
					cout << "convert image from 24 to 32 bit.\n";
				#endif
				bitmap = FreeImage_ConvertTo32Bits(bitmap);
				bpp = FreeImage_GetBPP(bitmap);
			}
			char* pixels = (char*)FreeImage_GetBits(bitmap);
			int s = w*h*4;
			for(int j= 0; j<s; j+=4){
				textura[j+0]= pixels[j+2];
				textura[j+1]= pixels[j+1];
				textura[j+2]= pixels[j+0];
				textura[j+3]= pixels[j+3];
			}
			glGenTextures(1, &id);
			glBindTexture(GL_TEXTURE_2D, id);
			#if DEBUG_IMAGE_LOAD
				cout << "bound texture.\n";
			#endif

			if(bpp == 24) {
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGB,GL_UNSIGNED_BYTE,(GLvoid*)textura );
			} else if(bpp == 32) {
				glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA, w, h, 0, GL_RGBA,GL_UNSIGNED_BYTE,(GLvoid*)textura );
			}
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
			glGenerateMipmap(GL_TEXTURE_2D);
			FreeImage_Unload(bitmap);//clear the png from memory
			//store the name of the file that was loaded
			tex = new texture(id,w,h);
		} else {
			failed();
			return;
		}


		//cout << "finished loading " << fileName << "\n";
		succeeded();
		return;
    }

	texture* tex;

public:

    load_image(string file_name,texture* t) :
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

#endif // IMAGE_LOAD_H

