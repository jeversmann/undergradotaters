#include "vcg.h"
#include "callgraph_walker.h"

// generate call graph
class vcgCCGWalker: public vcgWalker {
private:
  //typedef list<procNode*>            proc_list;
  typedef map <procNode*,node_list>  edge_map;

  node_list  nodes;
  procNode  *current_proc;
  string     current_proc_name; // the current calling context
  edge_map   edges;             // to avoid duplicate edges

  void at_proc( procNode *proc, Order ord ) {
    if(ord==Postorder) currently_excluded = false;
    else               currently_excluded = excluded(proc);
    if (ord == Postorder || currently_excluded) return;
    print_node(proc);

    declNode* decl = proc->decl();
    assert( decl );
    current_proc_name = decl->name();
    current_proc = proc;
  }

  void at_call( callNode *call, Order ord ) {
    // If the current procedure is requested to be excluded from the graph
    if (ord == Postorder || currently_excluded) return;
    if(call->proc())
      print_node(call->proc());
    else
      print_node(call);
    print_edge(current_proc, call->proc(), call);
  }

  void print_node(procNode *n) {
    // has the node been printed?
    if(find(nodes.begin(), nodes.end(), n) != nodes.end()) return;
    if(n->typ()!=Unit && excluded(n->coord())) return;                          

    nodes.push_front(n);

    graph << endl;
    graph << "   node: {" << endl;
    print_node_attribute( "title", node_title(n));
    print_node_attribute( "label", node_label(n) );
    graph << "   }" << endl;
  } // print_node(proc)

  // used for unknown callee.
  // for different calls, only one is printed if they have same node_title
  void print_node(callNode *c) {
    if(c->typ()!=Unit && excluded(c->coord())) return;                          

    // has the node been printed?
    string title = node_title(c);
    for(node_list_p n=nodes.begin(); n!=nodes.end(); n++)
      if(node_title(*n) == title) return;

    nodes.push_front(c);

    graph << endl;
    graph << "   node: {" << endl;
    print_node_attribute( "title", title);
    print_node_attribute( "label", node_label(c) );
    print_node_value( "shape", "ellipse" );
    graph << "   }" << endl;
  } // print_node(call)

  void print_edge(procNode *from, procNode *to, callNode *call) {
    node_list targets=edges[from];
    // has the edge been printed?
    if(to) {
      if(find(targets.begin(), targets.end(), to) != targets.end()) return;
      edges[from].push_front(to);
    } else {
      if(find(targets.begin(), targets.end(), call) != targets.end()) return;
      edges[from].push_front(call);
    }

    graph << endl;
    graph << "   edge: {";
    graph << "  sourcename: \"" << node_title(from) << "\"";
    if(to)
      graph << "  targetname: \"" << node_title(to)   << "\"";
    else
      graph << "  targetname: \"" << node_title(call)   << "\"";
    graph << "  label: \"" << call->coord().line()   << "\"";
    graph << "  }" << endl;
  } // print_edge

  string node_title(Node *n) {
    if(n->typ()==Call) return node_name(n);
    ostringstream title;
    char hex[16];
    sprintf(hex, "(%x)\0", n);
    title << node_name(n) << '@' << n->coord() << '.' << n->coord().offset()
          << hex << '\0';
    return title.str();
  } // node_title
 
  string node_label(Node *n) {
    if(n->typ()==Call) return node_name(n);
    ostringstream label;
    Coord coord = n->coord();
    label << node_name(n) << ':' << coord.line() << '.' << coord.offset()
          << '\0';
    return label.str();
  } // node_label

#define STRING(s1,s2) (string(s1 + string(" `") + s2 + "'"))

  string node_name(Node *n) {
    if(!n) return string("");
    switch(n->typ()) {
      case Proc: return STRING("Proc", ((procNode*)n)->decl()->name());
      case Call: {
        callNode *call = (callNode*) n;
        exprNode *cname = call->name();
        switch(cname->typ()) {
          case Id: return STRING("Call", ((idNode*) cname)->name());
          case Ptr: return "Call <Ptr>";
          case Unary: {
            ostringstream name;
            name <<"Call <Unary "<< ((unaryNode*)cname)->op()->print() <<">\0";
            return name.str();
          }
          default: {
            ostringstream name;
            name << "Call <unknown " << cname->typ() << ">\0";
            return name.str();
          }
        }
      }
      default: assert(false);
    }
  } // node_name

public:
  vcgCCGWalker(ostream& ostr, const string& comment, string excludefilename)
    : vcgWalker(ostr,comment,excludefilename), current_proc(NULL) {
    nodes.clear();  
    // Start graph description
    print_comment();
    print_comment( "=== C-Breeze Call Graph visualization ===" );
    print_comment();
    if( !comment.empty() ) {
      print_comment( comment );
      print_comment();
    }

    start_graph();
    print_graph_attribute( "orientation", "left_to_right" );
    print_graph_attribute( "display_edge_labels", "yes" );
  } // vcgCGWalker

  virtual ~vcgCCGWalker(void) { }
}; // class vcgCCGWalker


// phase for generating call graph
class vcgCCGPhase: public Phase {
private:
  string excludefilename;
public:
  vcgCCGPhase() {}
  virtual ~vcgCCGPhase() {}
  void run (void) {
    callgraph_walker::graph();

    string graph_file_name;

    if( CBZ::Program.empty() ) return;  // no units
    else if( CBZ::Program.size()==1 ) {
      // just one unit
       unitNode* u = CBZ::Program.front();
       graph_file_name = u->input_file() + ".callg";
    } else {
      cout << "Several compilation units present. Creating combined call graph."
           << endl;
      graph_file_name = "combined.callg";
    }

    // Open the output file.
    ofstream graph_file( graph_file_name.c_str() );
    
    if( graph_file ) {
      // Create the walker and process the ASTs of each unit.
      vcgCCGWalker callgw(graph_file, "File: " + graph_file_name,
                         excludefilename);

      for(unit_list_p i=CBZ::Program.begin(); i!=CBZ::Program.end(); ++i) {
        unitNode* u = *i;
        u->walk( callgw );  // build the graph
      }

    } else /* if( !graph_file ) */ {
      cerr << "vcgCGPhase: Can not open '" << graph_file_name
           << "' for writing." << endl;
    }
  }

  void get_flags(str_list_p & arg) { 
    string opt = *arg;
    if(strncmp("exclude:", opt.c_str(), 8) == 0) {
      excludefilename = opt;
      excludefilename.erase(0,8);
      arg++;
    }
  };
}; // vcgCGPhase

Phases vcgccgPhase( "vcgCCG", new vcgCCGPhase() );
