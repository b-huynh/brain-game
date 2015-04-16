//|||||||||||||||||||||||||||||||||||||||||||||||

#ifndef OGRE_DEMO_H
#define OGRE_DEMO_H

//|||||||||||||||||||||||||||||||||||||||||||||||

#include "OgreFramework.h"

//|||||||||||||||||||||||||||||||||||||||||||||||

#ifdef INCLUDE_RTSHADER_SYSTEM
#include "OgreRTShaderSystem.h"

/** This class demonstrates basic usage of the RTShader system.
 It sub class the material manager listener class and when a target scheme callback
 is invoked with the shader generator scheme it tries to create an equivalent shader
 based technique based on the default technique of the given material.
 */
class ShaderGeneratorTechniqueResolverListener : public Ogre::MaterialManager::Listener
{
public:
    
    ShaderGeneratorTechniqueResolverListener(Ogre::RTShader::ShaderGenerator* pShaderGenerator)
    {
        mShaderGenerator = pShaderGenerator;
    }
    
    /** This is the hook point where shader based technique will be created.
     It will be called whenever the material manager won't find appropriate technique
     that satisfy the target scheme name. If the scheme name is out target RT Shader System
     scheme name we will try to create shader generated technique for it.
     */
    virtual Ogre::Technique* handleSchemeNotFound(unsigned short schemeIndex,
                                                  const Ogre::String& schemeName, Ogre::Material* originalMaterial, unsigned short lodIndex,
                                                  const Ogre::Renderable* rend)
    {
        Ogre::Technique* generatedTech = NULL;
        
        // Case this is the default shader generator scheme.
        if (schemeName == Ogre::RTShader::ShaderGenerator::DEFAULT_SCHEME_NAME)
        {
            bool techniqueCreated;
            
            // Create shader generated technique for this material.
            techniqueCreated = mShaderGenerator->createShaderBasedTechnique(
                                                                            originalMaterial->getName(),
                                                                            Ogre::MaterialManager::DEFAULT_SCHEME_NAME,
                                                                            schemeName);
            
            // Case technique registration succeeded.
            if (techniqueCreated)
            {
                // Force creating the shaders for the generated technique.
                mShaderGenerator->validateMaterial(schemeName, originalMaterial->getName());
                
                // Grab the generated technique.
                Ogre::Material::TechniqueIterator itTech = originalMaterial->getTechniqueIterator();
                
                while (itTech.hasMoreElements())
                {
                    Ogre::Technique* curTech = itTech.getNext();
                    
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
    Ogre::RTShader::ShaderGenerator*	mShaderGenerator;			// The shader generator instance.
};
#endif

#include "Util.h"
#include "Networking.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
class OgreApp : public Ogre::RenderTargetListener
#else
class OgreApp : public OIS::KeyListener, public OIS::MouseListener, public Ogre::RenderTargetListener
#endif
{
public:
	OgreApp();
	~OgreApp();
    
    bool setName(const char* name);
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
	void startDemo(void* uiWindow, void* uiView, unsigned int width, unsigned int height, const char* name);
#else
	void startDemo(const char* name, MusicMode musica);
#endif
    void update(float elapsed);
    void endLevel(Evaluation forced = EVEN);
    void setLevel(Evaluation forced = EVEN, bool forward = true);
    
    float getTotalElapsed() const;
    void setPause(bool value);
    bool isPaused() const;
    bool predictSessionOver() const;
    void endGame();
    
    void requestResize();
    
    void activatePerformLeftMove();
    void activatePerformRightMove();
    void activatePerformDoubleTap(float x, float y);
    void activatePerformSingleTap(float x, float y);
    void activatePerformPinch();
    void activatePerformBeginLongPress();
    void activatePerformEndLongPress();
    
#if OGRE_PLATFORM != OGRE_PLATFORM_APPLE_IOS
    virtual bool mouseMoved(const OIS::MouseEvent &evt);
	virtual bool mousePressed(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
	virtual bool mouseReleased(const OIS::MouseEvent &evt, OIS::MouseButtonID id);
#endif
    
    virtual bool keyPressed(const OIS::KeyEvent &keyEventRef);
	virtual bool keyReleased(const OIS::KeyEvent &keyEventRef);
    virtual void preViewportUpdate(const Ogre::RenderTargetViewportEvent & evt) override;
    virtual void postViewportUpdate(const Ogre::RenderTargetViewportEvent & evt) override;
    
private:
    void setupDemoScene();
    void runDemo();
    bool initialiseRTShaderSystem(SceneManager* sceneMgr);
    void destroyRTShaderSystem();
    
    GameState gameState;
    MusicMode musicMode;
    
    unsigned seed;
    bool pause;
    bool sessionOver;
    Vector3 origin;
    SceneNode* lightNodeMain;
    SceneNode* backSphere;
    Tunnel* tunnel;
	Player* player;
    Hud* hud;
    LevelManager* levelMgr;
    
    bool					m_bShutdown;
#ifdef INCLUDE_RTSHADER_SYSTEM
    Ogre::RTShader::ShaderGenerator*			mShaderGenerator;			// The Shader generator instance.
    ShaderGeneratorTechniqueResolverListener*	mMaterialMgrListener;		// Shader generator material manager listener.
#endif // INCLUDE_RTSHADER_SYSTEM
    
};

//|||||||||||||||||||||||||||||||||||||||||||||||

#endif

//|||||||||||||||||||||||||||||||||||||||||||||||
