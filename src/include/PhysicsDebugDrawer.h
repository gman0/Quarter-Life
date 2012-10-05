#ifndef PHYSICS_DEBUG_DRAW
#define PHYSICS_DEBUG_DRAW

#include <bullet/btBulletDynamicsCommon.h>
#include <OGRE/Ogre.h>

using namespace Ogre;


typedef std::vector<Vector3> Vector3Array;



class DynamicRenderable : public SimpleRenderable
{
public:
  /// Constructor
  DynamicRenderable();
  /// Virtual destructor
  virtual ~DynamicRenderable();

  /** Initializes the dynamic renderable.
   @remarks
      This function should only be called once. It initializes the
      render operation, and calls the abstract function
      createVertexDeclaration().
   @param operationType The type of render operation to perform.
   @param useIndices Specifies whether to use indices to determine the
          vertices to use as input. */
  void initialize(RenderOperation::OperationType operationType,
                  bool useIndices);

  /// Implementation of SimpleRenderable
  virtual Real getBoundingRadius(void) const;
  /// Implementation of SimpleRenderable
  virtual Real getSquaredViewDepth(const Camera* cam) const;

protected:
  /// Maximum capacity of the currently allocated vertex buffer.
  size_t mVertexBufferCapacity;
  /// Maximum capacity of the currently allocated index buffer.
  size_t mIndexBufferCapacity;

  /** Creates the vertex declaration.
   @remarks
      Override and set mRenderOp.vertexData->vertexDeclaration here.
      mRenderOp.vertexData will be created for you before this method
      is called. */
  virtual void createVertexDeclaration() = 0;

  /** Prepares the hardware buffers for the requested vertex and index counts.
   @remarks
      This function must be called before locking the buffers in
      fillHardwareBuffers(). It guarantees that the hardware buffers
      are large enough to hold at least the requested number of
      vertices and indices (if using indices). The buffers are
      possibly reallocated to achieve this.
   @par
      The vertex and index count in the render operation are set to
      the values of vertexCount and indexCount respectively.
   @param vertexCount The number of vertices the buffer must hold.

   @param indexCount The number of indices the buffer must hold. This
          parameter is ignored if not using indices. */
  void prepareHardwareBuffers(size_t vertexCount, size_t indexCount);

  /** Fills the hardware vertex and index buffers with data.
   @remarks
      This function must call prepareHardwareBuffers() before locking
      the buffers to ensure the they are large enough for the data to
      be written. Afterwards the vertex and index buffers (if using
      indices) can be locked, and data can be written to them. */
  virtual void fillHardwareBuffers() = 0;
};




class DynamicLines : public DynamicRenderable
{
  typedef RenderOperation::OperationType OperationType;

public:
  /// Constructor - see setOperationType() for description of argument.
  DynamicLines(OperationType opType=RenderOperation::OT_LINE_STRIP);
  virtual ~DynamicLines();

  /// Add a point to the point list
  void addPoint(const Vector3 &p);
  /// Add a point to the point list
  void addPoint(Real x, Real y, Real z);

  /// Change the location of an existing point in the point list
  void setPoint(unsigned short index, const Vector3 &value);

  /// Return the location of an existing point in the point list
  const Vector3& getPoint(unsigned short index) const;

  /// Return the total number of points in the point list
  unsigned short getNumPoints(void) const;

  /// Remove all points from the point list
  void clear();

  /// Call this to update the hardware buffer after making changes.
  void update();

  /** Set the type of operation to draw with.
   * @param opType Can be one of
   *    - RenderOperation::OT_LINE_STRIP
   *    - RenderOperation::OT_LINE_LIST
   *    - RenderOperation::OT_POINT_LIST
   *    - RenderOperation::OT_TRIANGLE_LIST
   *    - RenderOperation::OT_TRIANGLE_STRIP
   *    - RenderOperation::OT_TRIANGLE_FAN
   *    The default is OT_LINE_STRIP.
   */
  void setOperationType(OperationType opType);
  OperationType getOperationType() const;

protected:
  /// Implementation DynamicRenderable, creates a simple vertex-only decl
  virtual void createVertexDeclaration();
  /// Implementation DynamicRenderable, pushes point list out to hardware memory
  virtual void fillHardwareBuffers();

private:
  std::vector<Vector3> mPoints;
  bool mDirty;
};



class PhysicsDebugDrawer : public btIDebugDraw
{
protected:
	SceneNode *mNode;
	btDynamicsWorld *mWorld;
	DynamicLines *mLineDrawer;
	bool mDebugOn;

public:

	PhysicsDebugDrawer(SceneNode *node, btDynamicsWorld *world);


	~PhysicsDebugDrawer();

	void step();

	void drawLine(const btVector3& from,const btVector3& to,const btVector3& color);

	void drawContactPoint(const btVector3& PointOnB,const btVector3& normalOnB,btScalar distance,int lifeTime,const btVector3& color);

	void reportErrorWarning(const char* warningString);

	void draw3dText(const btVector3& location,const char* textString);

	//0 for off, anything else for on.
	void setDebugMode(int isOn);

	//0 for off, anything else for on.
	int	getDebugMode() const;

};


#endif // PHYSICS_DEBUG_DRAW
