#include "PhysicsDebugDrawer.h"

using namespace Ogre;

//Converts from and to Bullet and Ogre stuff. Pretty self-explanatory.
class Convert
{
public:
	Convert() {};
	~Convert() {};

	static btQuaternion toBullet(const Ogre::Quaternion &q)
	{
		return btQuaternion(q.x, q.y, q.z, q.w);
	}
	static btVector3 toBullet(const Ogre::Vector3 &v)
	{
		return btVector3(v.x, v.y, v.z);
	}

	static Ogre::Quaternion toOgre(const btQuaternion &q)
	{
		return Ogre::Quaternion(q.w(), q.x(), q.y(), q.z());
	}
	static Ogre::Vector3 toOgre(const btVector3 &v)
	{
		return Ogre::Vector3(v.x(), v.y(), v.z());
	}
};


	DynamicRenderable::DynamicRenderable()
	{
	}
	//------------------------------------------------------------------------------------------------
	DynamicRenderable::~DynamicRenderable()
	{
	  delete mRenderOp.vertexData;
	  delete mRenderOp.indexData;
	}
	//------------------------------------------------------------------------------------------------
	void DynamicRenderable::initialize(RenderOperation::OperationType operationType,
									   bool useIndices)
	{
	  // Initialize render operation
	  mRenderOp.operationType = operationType;
	  mRenderOp.useIndexes = useIndices;
	  mRenderOp.vertexData = new VertexData;
	  if (mRenderOp.useIndexes)
		mRenderOp.indexData = new IndexData;

	  // Reset buffer capacities
	  mVertexBufferCapacity = 0;
	  mIndexBufferCapacity = 0;

	  // Create vertex declaration
	  createVertexDeclaration();
	}
	//------------------------------------------------------------------------------------------------
	void DynamicRenderable::prepareHardwareBuffers(size_t vertexCount,
												   size_t indexCount)
	{
	  // Prepare vertex buffer
	  size_t newVertCapacity = mVertexBufferCapacity;
	  if ((vertexCount > mVertexBufferCapacity) ||
		  (!mVertexBufferCapacity))
	  {
		// vertexCount exceeds current capacity!
		// It is necessary to reallocate the buffer.

		// Check if this is the first call
		if (!newVertCapacity)
		  newVertCapacity = 1;

		// Make capacity the next power of two
		while (newVertCapacity < vertexCount)
		  newVertCapacity <<= 1;
	  }
	  else if (vertexCount < mVertexBufferCapacity>>1) {
		// Make capacity the previous power of two
		while (vertexCount < newVertCapacity>>1)
		  newVertCapacity >>= 1;
	  }
	  if (newVertCapacity != mVertexBufferCapacity)
	  {
		mVertexBufferCapacity = newVertCapacity;
		// Create new vertex buffer
		HardwareVertexBufferSharedPtr vbuf =
		  HardwareBufferManager::getSingleton().createVertexBuffer(
			mRenderOp.vertexData->vertexDeclaration->getVertexSize(0),
			mVertexBufferCapacity,
			HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?

		// Bind buffer
		mRenderOp.vertexData->vertexBufferBinding->setBinding(0, vbuf);
	  }
	  // Update vertex count in the render operation
	  mRenderOp.vertexData->vertexCount = vertexCount;

	  if (mRenderOp.useIndexes)
	  {
		OgreAssert(indexCount <= std::numeric_limits<unsigned short>::max(), "indexCount exceeds 16 bit");

		size_t newIndexCapacity = mIndexBufferCapacity;
		// Prepare index buffer
		if ((indexCount > newIndexCapacity) ||
			(!newIndexCapacity))
		{
		  // indexCount exceeds current capacity!
		  // It is necessary to reallocate the buffer.

		  // Check if this is the first call
		  if (!newIndexCapacity)
			newIndexCapacity = 1;

		  // Make capacity the next power of two
		  while (newIndexCapacity < indexCount)
			newIndexCapacity <<= 1;

		}
		else if (indexCount < newIndexCapacity>>1)
		{
		  // Make capacity the previous power of two
		  while (indexCount < newIndexCapacity>>1)
			newIndexCapacity >>= 1;
		}

		if (newIndexCapacity != mIndexBufferCapacity)
		{
		  mIndexBufferCapacity = newIndexCapacity;
		  // Create new index buffer
		  mRenderOp.indexData->indexBuffer =
			HardwareBufferManager::getSingleton().createIndexBuffer(
			  HardwareIndexBuffer::IT_16BIT,
			  mIndexBufferCapacity,
			  HardwareBuffer::HBU_DYNAMIC_WRITE_ONLY); // TODO: Custom HBU_?
		}

		// Update index count in the render operation
		mRenderOp.indexData->indexCount = indexCount;
	  }
	}
	//------------------------------------------------------------------------------------------------
	Real DynamicRenderable::getBoundingRadius(void) const
	{
	  return Math::Sqrt(std::max(mBox.getMaximum().squaredLength(), mBox.getMinimum().squaredLength()));
	}
	//------------------------------------------------------------------------------------------------
	Real DynamicRenderable::getSquaredViewDepth(const Camera* cam) const
	{
	   Vector3 vMin, vMax, vMid, vDist;
	   vMin = mBox.getMinimum();
	   vMax = mBox.getMaximum();
	   vMid = ((vMax - vMin) * 0.5) + vMin;
	   vDist = cam->getDerivedPosition() - vMid;

	   return vDist.squaredLength();
	}
	



	enum {
	  POSITION_BINDING,
	  TEXCOORD_BINDING
	};
	//------------------------------------------------------------------------------------------------
	DynamicLines::DynamicLines(OperationType opType)
	{
	  initialize(opType,false);
	  setMaterial("BaseWhiteNoLighting");
	  mDirty = true;
	}
	//------------------------------------------------------------------------------------------------
	DynamicLines::~DynamicLines()
	{
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::setOperationType(OperationType opType)
	{
	  mRenderOp.operationType = opType;
	}
	//------------------------------------------------------------------------------------------------
	RenderOperation::OperationType DynamicLines::getOperationType() const
	{
	  return mRenderOp.operationType;
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::addPoint(const Vector3 &p)
	{
	   mPoints.push_back(p);
	   mDirty = true;
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::addPoint(Real x, Real y, Real z)
	{
	   mPoints.push_back(Vector3(x,y,z));
	   mDirty = true;
	}
	//------------------------------------------------------------------------------------------------
	const Vector3& DynamicLines::getPoint(unsigned short index) const
	{
	   assert(index < mPoints.size() && "Point index is out of bounds!!");
	   return mPoints[index];
	}
	//------------------------------------------------------------------------------------------------
	unsigned short DynamicLines::getNumPoints(void) const
	{
	  return (unsigned short)mPoints.size();
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::setPoint(unsigned short index, const Vector3 &value)
	{
	  assert(index < mPoints.size() && "Point index is out of bounds!!");

	  mPoints[index] = value;
	  mDirty = true;
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::clear()
	{
	  mPoints.clear();
	  mDirty = true;
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::update()
	{
	  if (mDirty) fillHardwareBuffers();
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::createVertexDeclaration()
	{
	  VertexDeclaration *decl = mRenderOp.vertexData->vertexDeclaration;
	  decl->addElement(POSITION_BINDING, 0, VET_FLOAT3, VES_POSITION);
	}
	//------------------------------------------------------------------------------------------------
	void DynamicLines::fillHardwareBuffers()
	{
	  int size = mPoints.size();

	  prepareHardwareBuffers(size,0);

	  if (!size) {
		mBox.setExtents(Vector3::ZERO,Vector3::ZERO);
		mDirty=false;
		return;
	  }

	  Vector3 vaabMin = mPoints[0];
	  Vector3 vaabMax = mPoints[0];

	  HardwareVertexBufferSharedPtr vbuf =
		mRenderOp.vertexData->vertexBufferBinding->getBuffer(0);

	  Real *prPos = static_cast<Real*>(vbuf->lock(HardwareBuffer::HBL_DISCARD));
	  {
	   for(int i = 0; i < size; i++)
	   {
		  *prPos++ = mPoints[i].x;
		  *prPos++ = mPoints[i].y;
		  *prPos++ = mPoints[i].z;

		  if(mPoints[i].x < vaabMin.x)
			 vaabMin.x = mPoints[i].x;
		  if(mPoints[i].y < vaabMin.y)
			 vaabMin.y = mPoints[i].y;
		  if(mPoints[i].z < vaabMin.z)
			 vaabMin.z = mPoints[i].z;

		  if(mPoints[i].x > vaabMax.x)
			 vaabMax.x = mPoints[i].x;
		  if(mPoints[i].y > vaabMax.y)
			 vaabMax.y = mPoints[i].y;
		  if(mPoints[i].z > vaabMax.z)
			 vaabMax.z = mPoints[i].z;
	   }
	  }
	  vbuf->unlock();

	  mBox.setExtents(vaabMin, vaabMax);

	  mDirty = false;
	}
	
	

	PhysicsDebugDrawer::PhysicsDebugDrawer(SceneNode *node, btDynamicsWorld *world)
		: mNode(node),
		  mWorld(world),
		  mDebugOn(true)
	{
		mLineDrawer = new DynamicLines(RenderOperation::OT_LINE_LIST);
		mNode->attachObject(mLineDrawer);

                if (!ResourceGroupManager::getSingleton().resourceGroupExists("BtOgre"))
                    ResourceGroupManager::getSingleton().createResourceGroup("BtOgre");
                if (!MaterialManager::getSingleton().resourceExists("BtOgre/DebugLines"))
                {
                    MaterialPtr mat = MaterialManager::getSingleton().create("BtOgre/DebugLines", "BtOgre");
                    mat->setReceiveShadows(false);
                    mat->setSelfIllumination(1,1,1);
                }

		mLineDrawer->setMaterial("BtOgre/DebugLines");
	}
	
	PhysicsDebugDrawer::~PhysicsDebugDrawer()
	{
                MaterialManager::getSingleton().remove("BtOgre/DebugLines");
                ResourceGroupManager::getSingleton().destroyResourceGroup("BtOgre");
		delete mLineDrawer;
	}
	
	void PhysicsDebugDrawer::step()
	{
		if (mDebugOn)
		{
			mWorld->debugDrawWorld();
			mLineDrawer->update();
			mNode->needUpdate();
			mLineDrawer->clear();
		}
		else
		{
			mLineDrawer->clear();
			mLineDrawer->update();
			mNode->needUpdate();
		}
	}
	
	void PhysicsDebugDrawer::drawLine(const btVector3& from,const btVector3& to,const btVector3& color)
	{
		mLineDrawer->addPoint(Convert::toOgre(from));
		mLineDrawer->addPoint(Convert::toOgre(to));
	}
	
	void PhysicsDebugDrawer::drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color)
	{
		mLineDrawer->addPoint(Convert::toOgre(PointOnB));
		mLineDrawer->addPoint(Convert::toOgre(PointOnB) + (Convert::toOgre(normalOnB) * distance * 20));
	}
	
	void PhysicsDebugDrawer::reportErrorWarning(const char* warningString)
	{
		LogManager::getSingleton().logMessage(warningString);
	}
	
	void PhysicsDebugDrawer::draw3dText(const btVector3& location,const char* textString)
	{
	}
	
	void PhysicsDebugDrawer::setDebugMode(int isOn)
	{
		mDebugOn = (isOn == 0) ? false : true;

		if (!mDebugOn)
			mLineDrawer->clear();
	}
	
	int	PhysicsDebugDrawer::getDebugMode() const
	{
		return mDebugOn;
	}
