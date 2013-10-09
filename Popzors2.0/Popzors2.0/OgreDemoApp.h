//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_DEMO_H
#define OGRE_DEMO_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.h"
#include "Pch.h"
#include "Ground.h"
#include "Poppy.h"
#include "Pot.h"
#include "Stage.h"
#include "Player.h"
#include "BaselinePattern.h"
#include "PotPattern.h"
#include <vector>

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef USE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"

/** This class demonstrates basic usage of the RTShader system.
 It sub class the material manager listener class and when a target scheme callback
 is invoked with the shader generator scheme it tries to create an equivalent shader
 based technique based on the default technique of the given material.
 */
class ShaderGeneratorTechniqueResolverListener : public MaterialManager::Listener
{
public:
    
	ShaderGeneratorTechniqueResolverListener(RTShader::ShaderGenerator* pShaderGenerator)
	{
		mShaderGenerator = pShaderGenerator;			
	}
    
	/** This is the hook point where shader based technique will be created.
     It will be called whenever the material manager won't find appropriate technique
     that satisfy the target scheme name. If the scheme name is out target RT Shader System
     scheme name we will try to create shader generated technique for it. 
     */
	virtual Technique* handleSchemeNotFound(unsigned short schemeIndex, 
                                                  const String& schemeName, Material* originalMaterial, unsigned short lodIndex,
                                                  const Renderable* rend)
	{	
		Technique* generatedTech = NULL;
        
		// Case this is the default shader generator scheme.
		if (schemeName == RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
		{
			bool techniqueCreated;
            
			// Create shader generated technique for this material.
			techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
                                                                            originalMaterial->getName(), 
                                                                            MaterialManager::DEFAULT_SCHEME_NAME, 
                                                                            schemeName);	
            
			// Case technique registration succeeded.
			if (techniqueCreated)
			{
				// Force creating the shaders for the generated technique.
				mShaderGenerator->validateMaterial(schemeName, originalMaterial->getName());
				
				// Grab the generated technique.
				Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();
                
				while (itTech.hasMoreElements())
				{
					Technique* curTech = itTech.getNext();
                    
					if (curTech->getSchemeName() == schemeName)
					{
						generatedTech = curTech;
						break;
					}
				}				
			}
		}
        
		return generatedTech;
	}
    
protected:	
	RTShader::ShaderGenerator*	mShaderGenerator;			// The shader generator instance.		
};
#endif

class DemoApp : public OIS::KeyListener, OIS::MouseListener
{
public:
	DemoApp();
	~DemoApp();
    
	void startDemo();
    void update(double elapsed);
	
	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
    bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
    
private:
    void setupDemoScene();
	void runDemo();
    bool initializeRTShaderSystem(SceneManager* sceneMgr);
    void finalizeRTShaderSystem();
    
    double totalElapsed;
    
    SceneNode*          lightNode;

    Stage* stage;
    Player* player;
    PopzorsPattern* pattern;
    
    //OgreOggISound* soundPickUp;
    
	bool					m_bShutdown;
#ifdef USE_RTSHADER_SYSTEM
    RTShader::ShaderGenerator*			mShaderGenerator;			// The Shader generator instance.
    ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;		// Shader generator material manager listener.	
#endif // USE_RTSHADER_SYSTEM

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||
