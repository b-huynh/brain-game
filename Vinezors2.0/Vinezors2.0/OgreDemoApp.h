//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_DEMO_H
#define OGRE_DEMO_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.h"

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

#include "Pod.h"
#include "TunnelSlice.h"
#include "Tunnel.h"
#include "Player.h"

class DemoApp : public OIS::KeyListener, public OIS::MouseListener, public Ogre::RenderTargetListener
{
public:
	DemoApp();
	~DemoApp();
    
	void startDemo();
    void update(double elapsed);

    bool mouseMoved(const OIS::MouseEvent &evt);
	bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);

	bool keyPressed(const OIS::KeyEvent &keyEventRef);
	bool keyReleased(const OIS::KeyEvent &keyEventRef);
    
    virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent & evt) override;
    virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt) override;
    
private:
    void setupDemoScene();
	void runDemo();
    bool initializeRTShaderSystem(SceneManager* sceneMgr);
    void finalizeRTShaderSystem();
    
    unsigned seed;
    bool pause;
    Vector3 origin;
    SceneNode* lightNodeMain;
    SceneNode* lightNodeSide;
    Tunnel* tunnel;
	Player* player;
    
    std::vector<Ogre::Overlay*> overlays;
    
    PanelOverlayElement* barHP;
    TextAreaOverlayElement* label1;
    TextAreaOverlayElement* label2;
    TextAreaOverlayElement* label3;
    TextAreaOverlayElement* label4;
    
    OgreOggISound* soundMusic;
    OgreOggISound* soundFeedbackGood;
    
	bool					m_bShutdown;
#ifdef USE_RTSHADER_SYSTEM
    RTShader::ShaderGenerator*			mShaderGenerator;			// The Shader generator instance.
    ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;		// Shader generator material manager listener.	
#endif // USE_RTSHADER_SYSTEM

};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif 

//|||||||||||||||||||||||||||||||||||||||||||||||
