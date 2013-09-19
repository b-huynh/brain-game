//
//  Util.cpp
//  Testing
//
//  Created by Calvin Phung on 8/27/13.
//
//

#include "Util.h"

#include <cstdlib>
#include <sstream>

int Util::randRangeInt(int min, int max)
{
    return min + rand () % (max - min + 1);
}

double Util::randRangeDouble(double min, double max)
{
    return min + static_cast<double>(rand()) / RAND_MAX * (max - min);
}

std::string Util::toStringInt(int value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

std::string Util::toStringDouble(double value)
{
    std::stringstream ss;
	ss << value;
	return ss.str();
}

// http://www.ogre3d.org/tikiwiki/tiki-index.php?page=manualspheremeshes
void Util::createSphere(const std::string& strName, const float r, const int nRings, const int nSegments)
{
    ManualObject * manual = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaRingAngle = (Math::PI / nRings);
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    unsigned short wVerticeIndex = 0 ;
    
    // Generate the group of rings for the sphere
    for( int ring = 0; ring <= nRings; ring++ ) {
        float r0 = r * sinf (ring * fDeltaRingAngle);
        float y0 = r * cosf (ring * fDeltaRingAngle);
        
        // Generate the group of segments for the current ring
        for(int seg = 0; seg <= nSegments; seg++) {
            float x0 = r0 * sinf(seg * fDeltaSegAngle);
            float z0 = r0 * cosf(seg * fDeltaSegAngle);
            
            // Add one vertex to the strip which makes up the sphere
            manual->position( x0, y0, z0);
            
            manual->normal(Vector3(x0, y0, z0).normalisedCopy());
            manual->textureCoord((float) seg / (float) nSegments, (float) ring / (float) nRings);
            
            if (ring != nRings) {
                // each vertex (except the last) has six indicies pointing to it
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex);
                manual->index(wVerticeIndex + nSegments);
                manual->index(wVerticeIndex + nSegments + 1);
                manual->index(wVerticeIndex + 1);
                manual->index(wVerticeIndex);
                wVerticeIndex ++;
            }
        }; // end for seg
    } // end for ring
    manual->end();
    MeshPtr mesh = manual->convertToMesh(strName);
    mesh->_setBounds( AxisAlignedBox( Vector3(-r, -r, -r), Vector3(r, r, r) ), false );
    
    mesh->_setBoundingSphereRadius(r);
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
}

void Util::createCylinder(const std::string& strName, const float r, const float h, const int nSegments)
{
    ManualObject * manual = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    
    float x0 = 0;
    float y0 = -h / 2;
    float y1 = h / 2;
    float z0 = 0;
    
    manual->position(x0, y0, z0);
    manual->normal(Vector3(x0, y0, z0).normalisedCopy());
    manual->textureCoord(0.0, 0.0);
    manual->position(x0, y1, z0);
    manual->normal(Vector3(x0, y1, z0).normalisedCopy());
    manual->textureCoord(0.0, 1.0);
    
    unsigned short botVerticeIndex = 0;
    unsigned short topVerticeIndex = 1;
    unsigned short iVerticeIndex = 2;
    unsigned short wVerticeIndex = iVerticeIndex;
    
    for (int seg = 0; seg <= nSegments; ++seg)
    {
        x0 = r * sinf(seg * fDeltaSegAngle);
        z0 = r * cosf(seg * fDeltaSegAngle);
        
        manual->position(x0, y0, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        manual->position(x0, y1, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        
        if (seg > 1)
        {
            manual->triangle(botVerticeIndex, wVerticeIndex + 2, wVerticeIndex);
            manual->triangle(topVerticeIndex, wVerticeIndex + 1, wVerticeIndex + 3);
            manual->triangle(wVerticeIndex, wVerticeIndex + 2, wVerticeIndex + 1);
            manual->triangle(wVerticeIndex + 2, wVerticeIndex + 3, wVerticeIndex + 1);
            
            wVerticeIndex += 2;
        }
    }
    manual->triangle(botVerticeIndex, iVerticeIndex, wVerticeIndex); // last triangles
    manual->triangle(topVerticeIndex, wVerticeIndex + 1, iVerticeIndex + 1);
    manual->triangle(wVerticeIndex, iVerticeIndex, wVerticeIndex + 1);
    manual->triangle(iVerticeIndex, iVerticeIndex + 1, wVerticeIndex + 1);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-r, -h / 2, -r);
    Vector3 tr = Vector3(r, h / 2, r);
    mesh->_setBounds( AxisAlignedBox( Vector3(-r, -h / 2, -r), Vector3(r, h / 2, r) ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length());
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
}

void Util::createUncappedCylinder(const std::string& strName, const float r, const float h, const int nSegments)
{
    ManualObject * manual = OgreFramework::getSingletonPtr()->m_pSceneMgr->createManualObject(strName);
    manual->begin("BaseWhiteNoLighting", RenderOperation::OT_TRIANGLE_LIST);
    
    float fDeltaSegAngle = (2 * Math::PI / nSegments);
    
    float x0 = 0;
    float y0 = -h / 2;
    float y1 = h / 2;
    float z0 = 0;
    
    manual->position(x0, y0, z0);
    manual->normal(Vector3(x0, y0, z0).normalisedCopy());
    manual->textureCoord(0.0, 0.0);
    manual->position(x0, y1, z0);
    manual->normal(Vector3(x0, y1, z0).normalisedCopy());
    manual->textureCoord(0.0, 1.0);
    
    unsigned short botVerticeIndex = 0;
    unsigned short topVerticeIndex = 1;
    unsigned short iVerticeIndex = 2;
    unsigned short wVerticeIndex = iVerticeIndex;
    
    for (int seg = 0; seg <= nSegments; ++seg)
    {
        x0 = r * sinf(seg * fDeltaSegAngle);
        z0 = r * cosf(seg * fDeltaSegAngle);
        
        manual->position(x0, y0, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        manual->position(x0, y1, z0);
        manual->normal(Vector3(x0, 0, z0).normalisedCopy());
        manual->textureCoord((float) seg / (float) nSegments, 0.5);
        
        if (seg > 1)
        {
            //manual->triangle(botVerticeIndex, wVerticeIndex + 2, wVerticeIndex);
            //manual->triangle(topVerticeIndex, wVerticeIndex + 1, wVerticeIndex + 3);
            manual->triangle(wVerticeIndex, wVerticeIndex + 2, wVerticeIndex + 1);
            manual->triangle(wVerticeIndex + 2, wVerticeIndex + 3, wVerticeIndex + 1);
            
            wVerticeIndex += 2;
        }
    }
    //manual->triangle(botVerticeIndex, iVerticeIndex, wVerticeIndex); // last triangles
    //manual->triangle(topVerticeIndex, wVerticeIndex + 1, iVerticeIndex + 1);
    manual->triangle(wVerticeIndex, iVerticeIndex, wVerticeIndex + 1);
    manual->triangle(iVerticeIndex, iVerticeIndex + 1, wVerticeIndex + 1);
    manual->end();
    
    MeshPtr mesh = manual->convertToMesh(strName);
    Vector3 bl = Vector3(-r, -h / 2, -r);
    Vector3 tr = Vector3(r, h / 2, r);
    mesh->_setBounds( AxisAlignedBox( Vector3(-r, -h / 2, -r), Vector3(r, h / 2, r) ), false );
    
    mesh->_setBoundingSphereRadius((tr - bl).length());
    unsigned short src, dest;
    if (!mesh->suggestTangentVectorBuildParams(VES_TANGENT, src, dest))
    {
        mesh->buildTangentVectors(VES_TANGENT, src, dest);
    }
}

void Util::generateMaterials()
{
    /*
    MaterialPtr mat0 =
    MaterialManager::getSingleton().create("PodUnknownMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat0->setLightingEnabled(true);
    mat0->setAmbient(0.3, 0.3, 0.3);
    mat0->setDiffuse(0.3, 0.3, 0.3, 1.0);
    mat0->load();
    
    MaterialPtr mat1 =
    MaterialManager::getSingleton().create("PodRedMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat1->setLightingEnabled(true);
    mat1->setAmbient(0.5, 0.0, 0.0);
    mat1->setDiffuse(0.5, 0.0, 0.0, 1.0);
    mat1->load();
    
    MaterialPtr mat2 =
    MaterialManager::getSingleton().create("PodGreenMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat2->setLightingEnabled(true);
    mat2->setAmbient(0.0, 0.5, 0.0);
    mat2->setDiffuse(0.0, 0.5, 0.0, 1.0);
    mat2->load();
    
    MaterialPtr mat3 =
    MaterialManager::getSingleton().create("PodBlueMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat3->setAmbient(0.0, 0.0, 0.5);
    mat3->setDiffuse(0.0, 0.0, 0.5, 1.0);
    mat3->load();
    
    MaterialPtr mat4 =
    MaterialManager::getSingleton().create("PodYellowMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat4->setLightingEnabled(true);
    mat4->setAmbient(0.5, 0.5, 0.0);
    mat4->setDiffuse(0.5, 0.5, 0.0, 1.0);
    mat4->load();
    
    MaterialPtr mat5 =
    MaterialManager::getSingleton().create("StemMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat5->setLightingEnabled(true);
    mat5->setAmbient(0.25, 0.5, 0.25);
    mat5->setDiffuse(0.25, 0.5, 0.25, 1.0);
    mat5->load();
    
    /*
    MaterialPtr mat6 =
    MaterialManager::getSingleton().create("PodShellMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat6->setLightingEnabled(true);
    mat6->setAmbient(0.5, 0.0, 0.0);
    mat6->setDiffuse(0.5, 0.0, 0.0, 0.1);
    mat6->load();
    */
    
    /*
     Technique *technique = m->createTechnique();
     Pass *pass = technique->createPass();
     pass->setIlluminationStage(Ogre::IS_PER_LIGHT);
     pass->setVertexColourTracking(Ogre::TVC_DIFFUSE);
     pass->setAmbient(1.0, 0.0, 0.0);
     pass->setLightingEnabled(true);
     */

}