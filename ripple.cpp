#include "ripple.h"
#include <soil/SOIL.h>

using glm::vec3;
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform2.hpp>

#include <stdio.h>
#include <iostream>
using namespace std;

Ripple::Ripple()
{
    drawBuf = 1;
}

void Ripple::initScene()
{
    compileAndLinkShader();
    glClearColor(0.1f, 0.1f, 0.1f, 0.0f);

    projection = mat4(1.0f);
    model = mat4(1.0f);

    initBuffers();
    //texID = loadTexture("");
    setMatrices();
}

void Ripple::initBuffers()
{
    nParticles = 961;
    nRows = 31;//nParticles=nRow**2
    nSize = 6.0f;

    float xsize, zsize;
    xsize = zsize = nSize;
    int xdivs, zdivs;
    xdivs = zdivs = nRows-1;

    faces = xdivs * zdivs;
    float* v = new float[3 * (xdivs + 1) * (zdivs + 1)];
    float* n = new float[3 * (xdivs + 1) * (zdivs + 1)];
    float* tex = new float[2 * (xdivs + 1) * (zdivs + 1)];
    unsigned int* el = new unsigned int[6 * xdivs * zdivs];

    float x2 = xsize / 2.0f;
    float z2 = zsize / 2.0f;
    float iFactor = (float)zsize / zdivs;
    float jFactor = (float)xsize / xdivs;
    float texi = 1.0f / zdivs;
    float texj = 1.0f / xdivs;
    float x, z;
    int vidx = 0, tidx = 0;
    for( int i = 0; i <= zdivs; i++ )
    {
        z = iFactor * i - z2;
        for( int j = 0; j <= xdivs; j++ )
        {
            x = jFactor * j - x2;
            v[vidx] = x;
            v[vidx+1] = 0.0f;
            v[vidx+2] = z;
            n[vidx] = 0.0f;
            n[vidx+1] = 1.0f;
            n[vidx+2] = 0.0f;
            vidx += 3;
            tex[tidx] = j * texi;
            tex[tidx+1] = i * texj;
            tidx += 2;
        }
    }
    v[1] = 10.0f;

    unsigned int rowStart, nextRowStart;
    int idx = 0;
    for( int i = 0; i < zdivs; i++ )
    {
        rowStart = i * (xdivs+1);
        nextRowStart = (i+1) * (xdivs+1);
        for( int j = 0; j < xdivs; j++ )
        {
            el[idx] = rowStart + j;
            el[idx+1] = nextRowStart + j;
            el[idx+2] = nextRowStart + j + 1;
            el[idx+3] = rowStart + j;
            el[idx+4] = nextRowStart + j + 1;
            el[idx+5] = rowStart + j + 1;
            idx += 6;
        }
    }
    //vertex position
    glGenBuffers(2, posBuf);glGenBuffers(1, &normalBuf);glGenBuffers(1, &texBuf);glGenBuffers(1, &elBuf);
    //---------------------------------
    glGenVertexArrays(2, particleArray);
    glBindVertexArray(particleArray[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*(xdivs+1)*(zdivs+1)*sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuf);
    glBufferData(GL_ARRAY_BUFFER, 3*(xdivs+1)*(zdivs+1)*sizeof(float), n, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glBufferData(GL_ARRAY_BUFFER, 3*(xdivs+1)*(zdivs+1)*sizeof(float), tex, GL_STATIC_DRAW);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*xdivs*zdivs*sizeof(unsigned int),el, GL_STATIC_DRAW);
    //--------------------------------


    /*glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glBufferData(GL_ARRAY_BUFFER, 3*(xdivs+1)*(zdivs+1)*sizeof(float), v, GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glBufferData(GL_ARRAY_BUFFER, 3*(xdivs+1)*(zdivs+1)*sizeof(float), v, GL_DYNAMIC_COPY);

    //vertex normal
    glGenBuffers(1, &normalBuf);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuf);
    glBufferData(GL_ARRAY_BUFFER, 3*(xdivs+1)*(zdivs+1)*sizeof(float), n, GL_STATIC_DRAW);

    //vertex texture
    glGenBuffers(1, &texBuf);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glBufferData(GL_ARRAY_BUFFER, 2*(xdivs+1)*(zdivs+1)*sizeof(float), tex, GL_STATIC_DRAW);

    //element
    glGenBuffers(1, &elBuf);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, elBuf);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6*xdivs*zdivs*sizeof(unsigned int), el, GL_STATIC_DRAW);
*/
    //left, top, right, bottom
    /*GLfloat* data = new GLfloat[nParticles*3];
    for(int i=0;i<nParticles*3;i++)
    {
        data[i]=0.0f;
    }
    glGenBuffers(1, &leftBuf);
    glBindBuffer(GL_ARRAY_BUFFER, leftBuf);
    glBufferData(GL_ARRAY_BUFFER, nParticles*3*sizeof(GLfloat), data, GL_DYNAMIC_COPY);
    glGenBuffers(1, &topBuf);
    glBindBuffer(GL_ARRAY_BUFFER, topBuf);
    glBufferData(GL_ARRAY_BUFFER, nParticles*3*sizeof(GLfloat), data, GL_DYNAMIC_COPY);
    glGenBuffers(1, &rightBuf);
    glBindBuffer(GL_ARRAY_BUFFER, rightBuf);
    glBufferData(GL_ARRAY_BUFFER, nParticles*3*sizeof(GLfloat), data, GL_DYNAMIC_COPY);
    glGenBuffers(1, &bottomBuf);
    glBindBuffer(GL_ARRAY_BUFFER, bottomBuf);
    glBufferData(GL_ARRAY_BUFFER, nParticles*3*sizeof(GLfloat), data, GL_DYNAMIC_COPY);

    delete [] data;*/
    delete [] v;
    delete [] n;
    delete [] tex;
    delete [] el;
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    //glBindBuffer(GL_ARRAY_BUFFER, 0);

    //assign to shaders
    //VAO
    /*glGenVertexArrays(2, particleArray);
    glBindVertexArray(particleArray[0]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[0]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuf);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/

    /*glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, leftBuf);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, topBuf);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, rightBuf);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(6);
    glBindBuffer(GL_ARRAY_BUFFER, bottomBuf);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/

    //another VAO
    /*glBindVertexArray(particleArray[1]);

    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, posBuf[1]);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, normalBuf);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, texBuf);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, NULL);*/

    /*glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, leftBuf);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, topBuf);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, rightBuf);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);

    glEnableVertexAttribArray(6);
    glBindBuffer(GL_ARRAY_BUFFER, bottomBuf);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/

    //feedback
    /*glGenTransformFeedbacks(2, feedback);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[0]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[0]);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[1]);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[1]);
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);*/

}

void Ripple::render(mat4 cameraView, mat4 cameraProjection)
{
    /*prog.setUniform("Calculate", true);
    glEnable(GL_RASTERIZER_DISCARD);

    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, feedback[drawBuf]);
    glBeginTransformFeedback(GL_POINTS);
    glBindVertexArray(particleArray[1-drawBuf]);
    glDrawElements(GL_TRIANGLES, 6*faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));
    glEndTransformFeedback();
    glDisable(GL_RASTERIZER_DISCARD);

    //read feedback
    GLfloat* posData = new GLfloat[nParticles*3];
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, posBuf[drawBuf]);
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, nParticles*3*sizeof(GLfloat), posData);
    cout<<"posData:"<<endl;
    for(int i=0;i<nParticles;i++)
    {
        cout<<i<<":"<<posData[3*i+1]<<" ";
    }
    cout<<endl;*/

    //render step
    glClear(GL_COLOR_BUFFER_BIT);
    //prog.setUniform("Calculate", false);
    view = cameraView;
    projection = cameraProjection;
    setMatrices();
    //glBindTexture(GL_TEXTURE_2D, texID);

    //calculate points around
    /*GLfloat* ambientData = new GLfloat[nParticles*3];
    int ambientDataSize = 3 * nParticles *sizeof(GLfloat);
    ambientData = left_points(posData);//left
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, leftBuf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ambientDataSize, ambientData);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    ambientData = top_points(posData);//top
    glEnableVertexAttribArray(4);
    glBindBuffer(GL_ARRAY_BUFFER, topBuf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ambientDataSize, ambientData);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    ambientData = right_points(posData);//right
    glEnableVertexAttribArray(5);
    glBindBuffer(GL_ARRAY_BUFFER, rightBuf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ambientDataSize, ambientData);
    glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, 0, NULL);
    ambientData = bottom_points(posData);//bottom
    glEnableVertexAttribArray(6);
    glBindBuffer(GL_ARRAY_BUFFER, bottomBuf);
    glBufferSubData(GL_ARRAY_BUFFER, 0, ambientDataSize, ambientData);
    glVertexAttribPointer(6, 3, GL_FLOAT, GL_FALSE, 0, NULL);*/

    //glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindVertexArray(particleArray[0]);
    glDrawElements(GL_TRIANGLES, 6*faces, GL_UNSIGNED_INT, ((GLubyte *)NULL + (0)));

    //swap drawBuf
    drawBuf = 1 - drawBuf;
}

void Ripple::setMatrices()
{
    mat4 mv = view * model;
    prog.setUniform("MVP", projection*mv);
}

void Ripple::compileAndLinkShader()
{
    if(!prog.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/waterripple/waterripple/shaders/ripple.vs",
                                   GLSLShader::VERTEX))
    {
        printf("Vertex Shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    if(!prog.compileShaderFromFile("/home/guanyuqing/Documents/cppcode/waterripple/waterripple/shaders/ripple.fs",
                                   GLSLShader::FRAGMENT))
    {
        printf("Fragment Shader failed to compile!\n%s",
               prog.log().c_str());
        exit(1);
    }
    GLuint progHandle = prog.getHandle();
    const char* outputNames[] = {"Position"};
    //glTransformFeedbackVaryings(progHandle, 1, outputNames, GL_SEPARATE_ATTRIBS);
    if(!prog.link())
    {
        printf("Shader program failed to link!\n%s",
               prog.log().c_str());
        exit(1);
    }
    prog.use();
}

GLuint Ripple::loadTexture(const char* texFile)
{
    int width, height;
    unsigned char* image = SOIL_load_image(texFile, &width, &height, 0, SOIL_LOAD_RGB);
    glActiveTexture(GL_TEXTURE0);
    GLuint texID;
    glGenTextures(1, &texID);
    glBindTexture(GL_TEXTURE_2D, texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return texID;
}

GLfloat* Ripple::left_points(GLfloat* data)
{
    GLfloat* left_data = new GLfloat[nParticles*3];
    for(int i=0;i<nParticles;i++)
    {
        if(i%nRows==0)
        {
            left_data[i*3] = data[i*3];
            left_data[i*3+1] = 0;
            left_data[i*3+2] = data[i*3+2];
        }
        else
        {
            left_data[i*3] = data[(i-1)*3];
            left_data[i*3+1] = data[(i-1)*3+1];
            left_data[i*3+2] = data[(i-1)*3+2];
        }
    }
    return left_data;
}

GLfloat* Ripple::top_points(GLfloat* data)
{
    GLfloat* top_data = new GLfloat[nParticles*3];
    for(int i=0;i<nParticles;i++)
    {
        if(i/nRows==0)
        {
            top_data[i*3] = data[i*3];
            top_data[i*3+1] = 0;
            top_data[i*3+2] = data[i*3+2];
        }
        else
        {
           top_data[i*3] = data[(i-nRows)*3];
           top_data[i*3+1] = data[(i-nRows)*3+1];
           top_data[i*3+2] = data[(i-nRows)*3+2];
        }
    }
    return top_data;
}

GLfloat* Ripple::right_points(GLfloat* data)
{
    GLfloat* right_data = new GLfloat[nParticles*3];
    for(int i=0;i<nParticles;i++)
    {
        if(i%(nRows-1)==0)
        {
            right_data[i*3] = data[i*3];
            right_data[i*3+1] = 0;
            right_data[i*3+2] = data[i*3+2];
        }
        else
        {
            right_data[i*3] = data[(i+1)*3];
            right_data[i*3+1] = data[(i+1)*3+1];
            right_data[i*3+2] = data[(i+1)*3+2];
        }
    }
    return right_data;
}

GLfloat* Ripple::bottom_points(GLfloat* data)
{
    GLfloat* bottom_data = new GLfloat[nParticles*3];
    for(int i=0;i<nParticles;i++)
    {
        if(i/nRows==nRows-1)
        {
            bottom_data[i*3] = data[i*3];
            bottom_data[i*3+1] = 0;
            bottom_data[i*3+2] = data[i*3+2];
        }
        else
        {
            bottom_data[i*3] = data[(i+nRows)*3];
            bottom_data[i*3+1] = data[(i+nRows)*3+1];
            bottom_data[i*3+2] = data[(i+nRows)*3+2];
        }
    }
    return bottom_data;
}










































