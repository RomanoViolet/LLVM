#ifndef TYPE_HIGH_ASSURANCE_COMPONENT
#define TYPE_HIGH_ASSURANCE_COMPONENT

class TypeHighAssuranceComponent
{
public:
  TypeHighAssuranceComponent( ) = default;
  virtual void doPreconditionCheck( ) = 0;
  virtual void doPostConditionCheck( ) = 0;
  virtual void initialize( ) = 0;
  virtual void compute( ) = 0;
  virtual ~TypeHighAssuranceComponent( ) = default;
};

#endif  // TYPE_HIGH_ASSURANCE_COMPONENT
