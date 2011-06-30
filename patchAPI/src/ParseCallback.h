/* PatchAPI uses the ParseAPI CFG to construct its own CFG, which 
   means that we need to be notified of any changes in the underlying
   CFG. These changes can come from a number of sources, including
   the PatchAPI modification interface or a self-modifying binary. 

   The PatchAPI modification chain looks like the following:
   1) User requests PatchAPI to modify the CFG;
   2) PatchAPI makes the corresponding request to ParseAPI;
   3) ParseAPI modifies its CFG, triggering "modified CFG" callbacks;
   4) PatchAPI hooks these callbacks and updates its structures.
   
   This is much easier than PatchAPI modifying them a priori, because
   it allows for self-modifying code (which skips step 2) to work
   with the exact same chain of events.
*/

#if !defined(_PATCHAPI_CALLBACK_H_)
#define _PATCHAPI_CALLBACK_H_

#include "parseAPI/h/ParseCallback.h"

namespace Dyninst {
namespace PatchAPI {

class PatchObject;

class PatchParseCallback : public ParseAPI::ParseCallback {
  public:
  PatchParseCallback(PatchObject *obj) : ParseAPI::ParseCallback(), _obj(obj) {};
   ~PatchParseCallback() {};
   
  protected:
   // Callbacks we want to know about: CFG mangling
   virtual void split_block_cb(ParseAPI::Block *, ParseAPI::Block *);
   virtual void destroy_cb(ParseAPI::Block *);
   virtual void destroy_cb(ParseAPI::Edge *);
   virtual void destroy_cb(ParseAPI::Function *);
   
   virtual void remove_edge_cb(ParseAPI::Block *, ParseAPI::Edge *, edge_type_t);
   virtual void add_edge_cb(ParseAPI::Block *, ParseAPI::Edge *, edge_type_t);
   
   virtual void remove_block_cb(ParseAPI::Function *, ParseAPI::Block *);
   virtual void add_block_cb(ParseAPI::Function *, ParseAPI::Block *);

  private:
   PatchObject *_obj;
};

};
};

#endif // _PATCHAPI_CALLBACK_H_
