/*an implimentation of the load class that loads an
.obj 3d plaintext file into a mesh class that can be
drawn in openGL.

I used several references to understand how the .obj
file format organizes data...
http://people.cs.clemson.edu/~dhouse/courses/405/docs/brief-obj-file-format.html

*/


#ifndef OBJ_LOAD_H
#define OBJ_LOAD_H
#include "../core/loader.h"
#include "../core/list.h"
#include "../graphics/mesh.h"
#include "../graphics/drawable.h"
#include "../graphics/texture.h"
#include <iostream>
#include <fstream>
#include <string>
#include "../core/string_util.h"


/*dependency types.

1 - image file*/




class load_obj : public loader {
private:

    /*store the temporary mesh that is created from
    loading the obj.*/
    list<vertex> v;
    list<vertex> vt;
    list<vertex> vn;
    list<polygon> p;

	list<material> m;

    mesh *p_m;
    mesh *c_m;

    void generate_mesh(mesh *m) {
        m->vertices = &v[0];
        m->vertexCount = v.getCount();
		m->polygons = &p[0];
		m->polygonCount = p.getCount();
        m->normals = &vn[0];
        m->normalCount = vn.getCount();
        m->texcoords = &vt[0];
        m->texcoordCount = vt.getCount();
    }


    /*seperates a string into three integers as seperated
    by forward slashes. This is used by the face line of the
    .obj file format to tell each vertex of a face what the
    vertex (position), texture coordinates, and vertex normal
    will be.*/
    void set_face_data(string code,int &v,int &vt,bool &has_vt,int &vn,bool &has_vn) {
        string token;
        if(!stringCutNext(code,token,"/")) return;
        v = stringToNumber<int>(token);
        if(!(has_vt = stringCutNext(code,token,"/"))) return;
        vt = stringToNumber<int>(token);
        if(!(has_vn = stringCutNext(code,token,"/"))) return;
        vn = stringToNumber<int>(token);
    }



    void custom_load(string file_name) {
		#if DEBUG_LOAD
        cout << "load::obj(" << file_name << ")\n";
		#endif
        //create a file stream
        ifstream file;
        string line;
        //attempt to open the file to read
        file.open(file_name.c_str());
        if(!file.good()) {
			cout << "file doesn't exist, bitch\n";
            failed();
            return;
        }
        /*set up all of the data objects required by the
        loop that goes through every line of the .obj so
        that they can be used without being reconstructed
        every time they are needed.*/
        string delimiter = " ";
        int v_ref 		= 0;
        int vt_ref 		= 0;
        int vn_ref 		= 0;
        bool has_vt 	= false;
        bool has_vn 	= false;
        int c_mat_ind	= 0;

		v = list<vertex>();
		vn = list<normal>();
		p = list<polygon>();
		vt = list<vertex>();
		bool got_polygon = false;

        while (!file.eof()) {
            /*get next line*/
            getline(file, line);
            /*skip any lines that are empty or start with
            the hash tag, which signifies a comment.*/
            if(line.length() < 1 || line[0] == '#'){
				if(line[0] == '#' && got_polygon){
					//store faces of mesh in current mesh
					/*add the vertices that were already defined before the mech
					existed to the mesh.*/
					generate_mesh(c_m);
					p_m->addChild(c_m);

					p = list<polygon>();
					v = list<vertex>();
					vn = list<normal>();
					vt = list<vertex>();

					got_polygon = false;
				}
				continue;
            }


            /*split the line by spaces to determine what
            type of information the line has.*/
            string token;
            double3 vert;
            /*make sure that at least one thing can be cut from
            the string*/
            //cout << " line :" << line << ": ";

            if(!stringCutNext(line,token," ")) continue;

            /*if the line represents a vertex*/
            if(token == "v") {
                if(stringGetDouble3(line,vert," ")) {
                    //cout << " - vertex (" << v.getCount() << ") " << vert.x << "," << vert.y << "," << vert.z << "\n";
                    v.push(vertex(vert.x,vert.y,vert.z));
                }

            }
            /*if the line represents a texture vertex*/
            else if(token == "vt") {
                if(stringGetDouble3(line,vert," ")) {
                    //cout << " - vertex texture (" << vt.getCount() << ") " << vert.x << "," << vert.y << "," << vert.z << "\n";
                    vt.push(vertex(vert.x,vert.y,vert.z));
                }
            }
            /*if the line represents a vertex normal.*/
			else if(token == "vn") {
                if(stringGetDouble3(line,vert," ")) {
                    //cout << " - vertex normal (" << vn.getCount() << ") " << vert.x << "," << vert.y << "," << vert.z << "\n";
                    vn.push(vertex(vert.x,vert.y,vert.z));
                }
            }
            else if(token == "p") {
            }
            else if(token == "s") {
            	double g;
				stringGetDouble(line,g," ");
				//cout << "smoothing group = " << g << " (not used) \n";
            }
            else if(token == "l") {
            }
            else if(token == "f") {

				list<int> verts = list<int>();
				list<int> norms = list<int>();
				list<int> txtrs = list<int>();
				//cout << "f : " << line << endl;
				/*parse out the vertex , texture, and normal information of the current face.
				it will return the indices of the vertices, textures, and normals already
				pulled from the file.*/
                while(stringCutNext(line,token," ")) {
                    set_face_data(token,v_ref,vt_ref,has_vt,vn_ref,has_vn);
					verts.push(v.getCount() + v_ref);
					if(has_vt) txtrs.push(vt.getCount() + vt_ref);
					else 	   txtrs.push(0);
					if(has_vn) norms.push(vn.getCount() + vn_ref);
					//cout << " vertex index:" << verts.last() << " normal index:" << norms.last() << " texture index: " << txtrs.last() << endl;
				}
                /*push the data to a polygon*/
                polygon np;
                np.vertex_index = &verts[0];
                np.normal_index = &norms[0];
                np.texcoord_index = &txtrs[0];
                np.mode = verts.getCount() > 3 ? GL_POLYGON : GL_TRIANGLES;
                np.vertex_count = verts.getCount();
                np.normal_count = norms.getCount();
				np.vertices = &v[0];
				np.normals = &vn[0];
				np.texcoords = &vt[0];
				np.mtl_index = c_mat_ind;
				p.push(np);
				got_polygon = true;
            }
			else if(token == "g" || token == "o") {
				c_m = new mesh();
				/*if the g has a length, it means it is the same group
				and is just giving it a name*/
				stringCutNext(line,token," ");
				//cout << "Name of group: " << token;
				c_m->name = token;
			}
			else if(token == "usemtl"){
				//cout << "usemtl : " << line << endl;
				if(!p_m->material_index->isSet(line)){
					//cout << " add new material : " << line << endl;
					material* n = new material();
					n->name = line;
					p_m->addMaterial(n);
				}
				//p.last().mtl_index = p_m->getMaterialIndex(line);
				c_mat_ind = p_m->getMaterialIndex(line);
				//cout << " get material id : " << c_mat_ind << endl;
			}
            /*if we don't know what to do with the row, just
            print it out. Once nothing is being printed to cmd,
            it means that the .obj file is fully understood.*/
            else {
                /*print out the line*/
                //cout << "? " << token << " :  " << line << endl;
            }

        }

        file.close();

        /*generate the mesh*/
        //generate_mesh();
        material *c_mat;

		/*set up all of the data object required to texture
		the object.*/
		string mat_fileName = file_name.substr(0,file_name.length()-4)+".mtl";
		string mat_fileDir = loader::settings.get<string>("default.directories.textures");
		//cout << "directory of .mtl is " << mat_fileDir << endl;
        file.open(mat_fileName.c_str());
        if(!file.good()) {
            cout << "texture library file failed to load\n";
        }
        else {
			//cout << "load texture library file\n";
			while (!file.eof()) {
				/*get next line*/
				getline(file, line);
				/*skip any lines that are empty or start with
				the hash tag, which signifies a comment.*/
				if(line.length() < 1 || line[0] == '#') continue;
				/*split the line by spaces to determine what
				type of information the line has.*/
				string token;
				double3 vert;
				/*make sure that at least one thing can be cut from
				the string*/
				line = stringTrim(line);
				if(!stringCutNext(line,token," ")) continue;
				/*if the line represents a vertex*/
				if(token == "newmtl") {
					//cout << "MATERIAL : " << line << "\n";
					c_mat = p_m->getMaterial(line);
					//cout << "New material : " << p_m->getMaterialIndex(line) << endl;
				}
				else if(token == "Ka") {
					//cout << " - ambient color: \n";
				}
				else if(token == "Kd") {
					//cout << " - diffuse color: \n";
					if(stringGetDouble3(line,vert," ")) {
						c_mat->color_diffuse = vert;
					}
				}
				else if(token == "Ks") {
					//cout << " - specular color: \n";
				}
				else if(token == "d" || token == "Tr") {
					//cout << " - transparency: \n";
				}
				else if(token == "illum") {
					//cout << " - illumination mode: \n";
				}
				else if(token == "map_Ka") {
					//cout << " - ambient map: \n";
				}
				else if(token == "map_Kd") {
					//strip the directory name from the file
					int chr;
					if(lastChar(line,'\\',chr)){
						line = line.substr(chr+1);
					}
					//cout << " - diffuse map: " << line << "\n";
					dependency* d = new dependency();
					d->type = 1;
					d->file = mat_fileDir + line;
					d->ref_data = (void*)c_mat;
					add_dependency(d);
					c_mat->map_diffuse = d->file;
				}
				else if(token == "map_Ks") {
					//cout << " - specular map: \n";
				}
				else if(token == "map_Ns") {
					//cout << " - specular highlight map: \n";
				}
				else if(token == "map_d") {
					//cout << " - alpha map: \n";
				}
				else if(token == "map_bump" || token == "bump") {
					//cout << " - bump map: \n";
				}
				/*if we don't know what to do with the row, just
				print it out. Once nothing is being printed to cmd,
				it means that the .obj file is fully understood.*/
				else {
					/*print out the line*/
					//cout << "? " << token << " :  " << line << endl;
				}
			}//end of while the file has more left to read
        }//end of if the obj file is readable
        succeeded();
    }


public:

    load_obj(string file_name,mesh* &m) :
        loader(file_name),
        p_m(m),
        c_m(NULL){
		m = new mesh();
    }


	void integrateDependency(dependency* depends){
		material* mat = (material*)depends->ref_data;
		texture* tex = (texture*)depends->load_data;
		/*assign the gl texture id to the correct map in the material that
		it was loaded for.*/
		switch(depends->type){
		case 1:	//diffuse map
			mat->diffuse_id = tex->id;
		}
	}

    mesh* get_mesh(){
        return p_m;
    }


	void* getData(){ return (void*) p_m; }

};

#endif // OBJ_LOAD_H
