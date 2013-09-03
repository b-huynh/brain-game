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

void Util::generateMaterials()
{
    MaterialPtr mat0 =
    MaterialManager::getSingleton().create("GeneralMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat0->setLightingEnabled(true);
    mat0->load();
    
    MaterialPtr mat1 =
    MaterialManager::getSingleton().create("EarthMaterial", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat1->setLightingEnabled(true);
    mat1->setAmbient(0.0, 0.0, 1.0);
    mat1->setDiffuse(0.0, 1.0, 0.0, 1.0);
    mat1->load();
    
    MaterialPtr mat2 =
    MaterialManager::getSingleton().create("GeneralMaterial1", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat2->setLightingEnabled(true);
    mat2->load();
    
    MaterialPtr mat3 =
    MaterialManager::getSingleton().create("GeneralMaterial2", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat3->setLightingEnabled(true);
    mat3->load();
    
    MaterialPtr mat4 =
    MaterialManager::getSingleton().create("GeneralMaterial3", ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, true);
    mat4->setLightingEnabled(true);
    mat4->load();
    
    /*
     Technique *technique = m->createTechnique();
     Pass *pass = technique->createPass();
     pass->setIlluminationStage(Ogre::IS_PER_LIGHT);
     pass->setVertexColourTracking(Ogre::TVC_DIFFUSE);
     pass->setAmbient(1.0, 0.0, 0.0);
     pass->setLightingEnabled(true);
     */

}