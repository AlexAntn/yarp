/*
 *  Yarp Modules Manager
 *  Copyright: 2011 (C) Robotics, Brain and Cognitive Sciences - Italian Institute of Technology (IIT)
 *  Authors: Ali Paikan <ali.paikan@iit.it>
 * 
 *  Copy Policy: Released under the terms of the LGPLv2.1 or later, see LGPL.TXT
 *
 */

#if defined(WIN32)
    #pragma warning (disable : 4250)
    #pragma warning (disable : 4520)
#endif

#include "module_property_window.h"
#include "icon_res.h"
#include "ymm-dir.h"

#include "main_window.h"
#include "arrow_model.h"

ModulePropertyWindow::ModulePropertyWindow(MainWindow* parent, 
                               Manager* manager) : m_pModule(NULL)
{   
    m_pParent = parent;
    m_pManager = manager;
    m_pModule = NULL;
//    m_pIModule = NULL;

    /* Create a new scrolled window, with scrollbars only if needed */
    set_policy(Gtk::POLICY_AUTOMATIC, Gtk::POLICY_AUTOMATIC);
    add(m_TreeView);

    /* create tree store */
    m_refTreeModel = Gtk::TreeStore::create(m_Columns);
    m_TreeView.set_model(m_refTreeModel);

    //Add the Model’s column to the View’s columns: 
    Gtk::CellRendererText* itemRenderer = Gtk::manage(new Gtk::CellRendererText());
    itemRenderer->property_editable() = false;
    Gtk::TreeViewColumn* itemCol = Gtk::manage(new Gtk::TreeViewColumn("Property", *itemRenderer));
    itemCol->add_attribute(*itemRenderer, "foreground-gdk", m_Columns.m_col_color_item);
    itemCol->add_attribute(*itemRenderer, "text", m_Columns.m_col_name);
    itemCol->set_resizable(true);
    m_TreeView.append_column(*itemCol);



    Gtk::CellRendererText* valueRenderer = Gtk::manage(new Gtk::CellRendererText());
    valueRenderer->property_editable() = false;
    Gtk::TreeViewColumn* valueCol = Gtk::manage(new Gtk::TreeViewColumn("Value", *valueRenderer));
    valueCol->add_attribute(*valueRenderer, "foreground-gdk", m_Columns.m_col_color_value);
    valueCol->add_attribute(*valueRenderer, "text", m_Columns.m_col_value);
    valueCol->set_resizable(true);
    valueRenderer->property_editable() = true;    
    //valueCol->add_attribute(*valueRenderer, "editable", m_Columns.m_col_editable);   

    //valueCol->set_cell_data_func(*valueRenderer, sigc::mem_fun(*this,
    //    &ModulePropertyWindow::onCellData) );
    valueRenderer->signal_edited().connect( sigc::mem_fun(*this,
              &ModulePropertyWindow::onCellEdited) );
    m_TreeView.append_column(*valueCol);


    //m_TreeView.append_column_editable("Value", m_Columns.m_col_value);
    //m_TreeView.get_column(1)->set_resizable(true);
       //Tell the view column how to render the model values:
    //m_TreeView.set_property("enable_grid_lines", true);
    m_TreeView.set_grid_lines(Gtk::TREE_VIEW_GRID_LINES_BOTH);
    m_TreeView.set_rules_hint(true);
    show_all_children();
}


ModulePropertyWindow::~ModulePropertyWindow()
{
}

void ModulePropertyWindow::onRefresh()
{
    if(!m_pModule)
        return;
    update(m_pModule);
    m_pParent->reportErrors();
}

void ModulePropertyWindow::onTabCloseRequest() 
{ 
    m_pParent->onTabCloseRequest(this); 
}

void ModulePropertyWindow::update(Module* module)
{
    //m_pIModule = NULL;
    m_pModule = module;
    /*
    Application* application = m_pManager->getKnowledgeBase()->getApplication();            
    if(application)
    {
        for(int i=0; i<application->imoduleCount(); i++)
            if(strcmp(application->getImoduleAt(i).getTag(), module->getLabel()) == 0)
            {
                m_pIModule = &(application->getImoduleAt(i));
                break;
            }
    }
    
    if(!m_pIModule)
        return;
    */

    //Module* m_pModule = ModulePropertyWindow::m_pModule;
    m_refTreeModel->clear();

    Gtk::TreeModel::Row row;
    Gtk::TreeModel::Row childrow;
    Gtk::TreeModel::Row cchildrow;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Name";
    row[m_Columns.m_col_value] = m_pModule->getName();
    row[m_Columns.m_col_color_value] = Gdk::Color("#888888");
    row[m_Columns.m_col_editable] = false;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Description";
    row[m_Columns.m_col_value] = m_pModule->getDescription();
    row[m_Columns.m_col_color_value] = Gdk::Color("#888888");
    row[m_Columns.m_col_editable] = false;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Node";
    row[m_Columns.m_col_value] = m_pModule->getHost();
    row[m_Columns.m_col_editable] = true;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Stdio";
    row[m_Columns.m_col_value] = m_pModule->getStdio();
    row[m_Columns.m_col_editable] = true;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Workdir";
    row[m_Columns.m_col_value] = m_pModule->getWorkDir();
    row[m_Columns.m_col_editable] = true;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Prefix";
    row[m_Columns.m_col_value] = m_pModule->getPrefix();
    row[m_Columns.m_col_editable] = true;

    row = *(m_refTreeModel->append());
    row[m_Columns.m_col_name] = "Parameters";
    row[m_Columns.m_col_value] = m_pModule->getParam();
    row[m_Columns.m_col_color_value] = Gdk::Color("#888888");
    row[m_Columns.m_col_editable] = false;

    for(int i=0; i<m_pModule->argumentCount(); i++)
    {
        childrow = *(m_refTreeModel->append(row.children()));
        childrow[m_Columns.m_col_name] = m_pModule->getArgumentAt(i).getParam();
        childrow[m_Columns.m_col_value] = m_pModule->getArgumentAt(i).getValue();
        Glib::ustring strval = childrow[m_Columns.m_col_value];
        if(m_pModule->getArgumentAt(i).isRequired() 
            && !strval.size())
            childrow[m_Columns.m_col_color_item] = Gdk::Color("#BF0303");
        childrow[m_Columns.m_col_editable] = true;
    }
    
    //updateParamteres();

    m_TreeView.expand_all();

}

/*
bool ApplicationWindow::getRowByName(const char* name, Gtk::TreeModel::Row* row)
{
    typedef Gtk::TreeModel::Children type_children;
    type_children children = m_refTreeModel->children();
    for(type_children::iterator iter = children.begin(); iter!=children.end(); ++iter)
    {
        Glib::ustring strName = (*iter)[m_modColumns.m_col_name];
        if( strName == name)
        {
            *row = (*iter);
            return true;
        }
    }
    return false;
}
*/
/*
Connection* ModulePropertyWindow::findConnection( CnnContainer& connections, const char* szPort, bool from)
{
    CnnIterator jtr;
    for(jtr=connections.begin(); jtr!=connections.end(); jtr++)    
    {
        if(from && (string((*jtr).from()) == string(szPort)))
            return &(*jtr);        
        if(!from && (string((*jtr).to()) == string(szPort)))
            return &(*jtr);
    }

    return NULL;
}
*/

void ModulePropertyWindow::updateModule(const char* item, const char* value)
{
    if(!m_pModule /*|| !m_pIModule*/)
        return;

    // updating modules
    if(strcmp(item, "Node") == 0)
    {
        m_pModule->setHost(value);
        //if(m_pIModule) m_pIModule->setHost(value);
    }
    else if(strcmp(item,"Stdio") == 0)
    {
        m_pModule->setStdio(value);
        //if(m_pIModule) m_pIModule->setStdio(value);

    }
    else if(strcmp(item, "Workdir") == 0)
    {
        m_pModule->setWorkDir(value);
        //if(m_pIModule) m_pIModule->setWorkDir(value);

    }
    else if(strcmp(item, "Prefix") == 0)
    {    
        //if(m_pIModule)  
        //    m_pIModule->setPrefix(value);

        string strPrefix;
        Application* application = m_pManager->getKnowledgeBase()->getApplication(); 
        if(application)
        {            
            strPrefix = string(application->getPrefix()) + string(value); 
            for(int j=0; j<m_pModule->outputCount(); j++)
            {
                OutputData *output = &m_pModule->getOutputAt(j);

                for(int i=0; i<application->connectionCount(); i++)
                {
                    Connection con = application->getConnectionAt(i);
                    Connection updatedCon = con;
                    if(con.getCorOutputData())
                    {
                        if(con.getCorOutputData() == output)
                        {
                            string strFrom = strPrefix + string(output->getPort());
                            updatedCon.setFrom(strFrom.c_str());
                            m_pManager->getKnowledgeBase()->updateConnectionOfApplication(application, 
                                                        con, updatedCon);
                            // updating arrow's connection
                            if(dynamic_cast<ArrowModel*>(con.getModel()))
                                dynamic_cast<ArrowModel*>(con.getModel())->setConnection(updatedCon);
                        }
                    }
                }
            }

            for(int j=0; j<m_pModule->inputCount(); j++)
            {
                InputData *input = &m_pModule->getInputAt(j);

                for(int i=0; i<application->connectionCount(); i++)
                {
                    Connection con = application->getConnectionAt(i);
                    Connection updatedCon = con;
                    if(con.getCorInputData())
                    {
                        if(con.getCorInputData() == input)
                        {
                            string strTo = strPrefix + string(input->getPort());
                            updatedCon.setTo(strTo.c_str());
                            m_pManager->getKnowledgeBase()->updateConnectionOfApplication(application, 
                                                        con, updatedCon);
                            // updating arrow's connection
                            if(dynamic_cast<ArrowModel*>(con.getModel()))
                                dynamic_cast<ArrowModel*>(con.getModel())->setConnection(updatedCon);
                        }
                    }
                }
            }             
        }
        else
            strPrefix = value;
        m_pModule->setPrefix(strPrefix.c_str());
    }
    else if(strcmp(item, "Parameters") == 0)
    {
        m_pModule->setParam(value);
        //if(m_pIModule) m_pIModule->setParam(value);
    }
}

void ModulePropertyWindow::onCellEdited(const Glib::ustring& path_string, 
                    const Glib::ustring& new_text)
{
    if(!m_pModule /*|| !m_pIModule*/ )
        return;

    Gtk::TreePath path(path_string);

    //Get the row from the path:
    Gtk::TreeModel::iterator iter = m_refTreeModel->get_iter(path);
    if(iter)
    {
        Gtk::TreeModel::Row row = *iter;
        //Put the new value in the model:
        Glib::ustring strName = Glib::ustring(row[m_Columns.m_col_name]);

        if((strName != "Parameters") && 
         (strName != "Name") && 
         (strName != "Description"))
            row[m_Columns.m_col_value] = new_text;
       
        updateParamteres();
        updateModule(strName.c_str(), new_text.c_str());

   }
}

void ModulePropertyWindow::updateParamteres()
{
    // updating parameters
    typedef Gtk::TreeModel::Children type_children;
    type_children children = m_refTreeModel->children();
    ostringstream strParams;
    Glib::ustring strName;
    for(type_children::iterator iter = children.begin(); iter!=children.end(); ++iter)
    {
        strName = Glib::ustring((*iter)[m_Columns.m_col_name]);
        if(strName == "Parameters")
        {
            for(type_children::iterator jter = (*iter).children().begin(); 
                jter!=(*iter).children().end(); ++jter)
            {
                Glib::ustring strItem = Glib::ustring((*jter)[m_Columns.m_col_name]);
                Glib::ustring strValue = Glib::ustring((*jter)[m_Columns.m_col_value]);
                for(int i=0; i<m_pModule->argumentCount(); i++) 
                {
                    if(strItem == m_pModule->getArgumentAt(i).getParam())
                    {   
                        if(strValue.size())
                        {
                            if(!m_pModule->getArgumentAt(i).isSwitch())                            
                                strParams<<"--"<<strItem<<" "<<strValue<<" ";
                            else
                            {
                                if(compareString(strValue.c_str(), "on" ))
                                    strParams<<"--"<<strItem<<" ";
                                else
                                    (*jter)[m_Columns.m_col_value] = "off";
                            }
                        }
                        else
                            if(m_pModule->getArgumentAt(i).isSwitch())                            
                                (*jter)[m_Columns.m_col_value] = "off";
                    }    

                    if((strItem == m_pModule->getArgumentAt(i).getParam()) &&
                        m_pModule->getArgumentAt(i).isRequired())
                    {    
                        if(!strValue.size())
                            (*jter)[m_Columns.m_col_color_item] = Gdk::Color("#BF0303");
                        else
                            (*jter)[m_Columns.m_col_color_item] = Gdk::Color("#000000");
                    }
                }
            }
            (*iter)[m_Columns.m_col_value] = strParams.str();
            updateModule(strName.c_str(), strParams.str().c_str());
            break;
        }
    }        
}
