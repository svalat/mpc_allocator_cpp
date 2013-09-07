#ifndef TOPO_PTR_H
#define TOPO_PTR_H

/********************  HEADERS  *********************/

/********************  NAMESPACE  *******************/
namespace MPCAllocator
{

/*********************  CLASS  **********************/
class TopoObject
{
	public:
		TopoObject(const char * levelType,int subDepth,int ratio = 1);
		virtual ~TopoObject(void);
		void * getLocalObject(void);
		void * getObjet(int id);
	protected:
		virtual void * allocObject(int id) = 0;
	private:
		int ratio;
		int levelType;
		int subDepth;
		void ** objs;
		int count;
};

/*********************  CLASS  **********************/
template <class T>
class GenericTopoObject : public TopoObject
{
	public:
		TopoObject(const char * levelType,int subDepth,int ratio = 1);
		T * getLocalObject(void);
		T * getObject(int id);
	protected:
		virtual void* allocObject ( int id );
};

};

#endif //TOPO_PTR_H
