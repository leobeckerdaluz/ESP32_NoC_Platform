#include <iostream>
#include <thread>
#include <vector>
#include "defines.h"
#include <stdio.h>

void generate_file(AllStruct all_info){
    printf("-----------------------------\n");
    printf("--------- Out File: ---------\n");
    printf("-----------------------------\n");

    
    FILE *pFile = fopen(OUTPUT_FILE, "w");


    // Headers
    #ifdef XML
    printf("<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    fprintf(pFile,"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n");
    #endif
    #ifdef JSON
    printf("{\n");
    fprintf(pFile,"{\n");
    #endif

    // START Project
    #ifdef JSON
    printf("\"project\":\n");
    printf("\t{\n");
    fprintf(pFile,"\"project\":\n");
    fprintf(pFile, "\t\t{\n");
    #endif
    #ifdef XML
    printf("<project>\n");
    fprintf(pFile,"<project>\n");
    #endif

    // START NoC
    #ifdef JSON
    printf("\t\"noc\":\n");
    printf("\t\t{\n");
    fprintf(pFile, "\t\t\"noc\":\n");
    fprintf(pFile, "\t\t{\n");
    #endif
    #ifdef XML
    printf("\t<noc>\n");
    fprintf(pFile, "\t\t<noc>\n");
    fprintf(pFile, "\t\t\t<x>%s</x>\n", (all_info.x).c_str());
    fprintf(pFile, "\t\t\t<y>%s</y>\n",(all_info.y).c_str());
    fprintf(pFile, "\t\t\t<topology>%s</topology>\n",(all_info.topologia).c_str());
    #endif

    // --------------------------
    // ------- Routers -------
    // --------------------------

    #ifdef XML
    printf("\t\t<routers>\n");
    fprintf(pFile, "\t\t\t<routers>\n");

    for (int i=0; i<all_info.all_PEs_inside.size(); i++){
        fprintf(pFile, "\t\t\t\t<node>\n");
        fprintf(pFile, "\t\t\t\t\t<id>%d</id>\n", all_info.all_PEs_inside[i].id);
        if (all_info.all_PEs_inside[i].top != BIG_NUMBER)    
            fprintf(pFile, "\t\t\t\t\t<top>%d</top>\n", all_info.all_PEs_inside[i].top);
        if (all_info.all_PEs_inside[i].right != BIG_NUMBER)    
            fprintf(pFile, "\t\t\t\t\t<right>%d</right>\n", all_info.all_PEs_inside[i].right);
        if (all_info.all_PEs_inside[i].left != BIG_NUMBER)
            fprintf(pFile, "\t\t\t\t\t<left>%d</left>\n", all_info.all_PEs_inside[i].left);
        if (all_info.all_PEs_inside[i].bottom != BIG_NUMBER)    
            fprintf(pFile, "\t\t\t\t\t<bottom>%d</bottom>\n", all_info.all_PEs_inside[i].bottom);
        fprintf(pFile, "\t\t\t\t</node>\n");
    }

    fprintf(pFile, "\t\t\t</routers>\n");
    printf("\t\t</routers>\n");
    #endif

    #ifdef JSON
    printf("\t\t\"routers\":\n");
    fprintf(pFile, "\t\t\t\"routers\":\n");
    fprintf(pFile, "\t\t\t\[\n");
    for (int i=0; i<all_info.all_PEs_inside.size(); i++){
        fprintf(pFile, "\t\t\t\t{\n");
        fprintf(pFile, "\t\t\t\t\t\"id\":\"%d\"", all_info.all_PEs_inside[i].id);
        if (all_info.all_PEs_inside[i].top != BIG_NUMBER)    
            fprintf(pFile, ",\n\t\t\t\t\"top\":\"%d\"", all_info.all_PEs_inside[i].top);
        if (all_info.all_PEs_inside[i].right != BIG_NUMBER)    
            fprintf(pFile, ",\n\t\t\t\t\"right\":\"%d\"", all_info.all_PEs_inside[i].right);
        if (all_info.all_PEs_inside[i].left != BIG_NUMBER)
            fprintf(pFile, ",\n\t\t\t\t\"left\":\"%d\"", all_info.all_PEs_inside[i].left);
        if (all_info.all_PEs_inside[i].bottom != BIG_NUMBER)    
            fprintf(pFile,",\n\t\t\t\t\"bottom\":\"%d\"", all_info.all_PEs_inside[i].bottom);
        

        if(i == all_info.PEs_communication_inside.size() -1){    
            fprintf(pFile,"\n\t\t\t}\n");
        }else
        {
            fprintf(pFile,"\n\t\t\t},\n");
        }
    }
    fprintf(pFile, "\t\t\t]\n");
    fprintf(pFile, "\t\t},\n");

    #endif

    // --------------------------
    // ------- End Routers ------
    // --------------------------
    // END NoC
    #ifdef JSON
    printf("\t\t}}\n");
    #endif
    #ifdef XML
    printf("\t</noc>\n");
    fprintf(pFile, "\t\t</noc>\n");
    #endif
    
    // --------------------------
    // ------ communication -----
    // --------------------------
    #ifdef XML
    fprintf(pFile, "\t\t<communication>\n");
    fprintf(pFile, "\t\t\t<volume>\n");
    Communication aux;
    int sum;
    for (int j=0; j<all_info.PEs_communication_inside.size(); j++){
        aux = all_info.PEs_communication_inside[j];
        sum = aux.cont_msgs_top + aux.cont_msgs_bottom + aux.cont_msgs_right + aux.cont_msgs_left + aux.cont_msgs_tantofaz;
        if(!sum)
            sum = 1;
        fprintf(pFile, "\t\t\t\t<router>\n");
        fprintf(pFile, "\t\t\t\t\t<id>%d</id>\n", all_info.PEs_communication_inside[j].id);
        fprintf(pFile, "\t\t\t\t\t<message_count>%d</message_count>\n", sum);
        fprintf(pFile, "\t\t\t\t\t<ports>\n");
        fprintf(pFile,"\t\t\t\t\t\t<top>%.2f%%</top>\n", aux.cont_msgs_top*100.0/sum);
        fprintf(pFile,"\t\t\t\t\t\t<bottom>%.2f%%</bottom>\n", aux.cont_msgs_bottom*100.0/sum);
        fprintf(pFile,"\t\t\t\t\t\t<left>%.2f%%</left>\n", aux.cont_msgs_left * 100.0/sum);
        fprintf(pFile,"\t\t\t\t\t\t<right>%.2f%%</right>\n", aux.cont_msgs_right * 100.0/sum);
        fprintf(pFile, "\t\t\t\t\t</ports>\n");
        fprintf(pFile, "\t\t\t\t</router>\n");
    }
    fprintf(pFile, "\t\t\t</volume>\n");
    fprintf(pFile, "\t\t</communication>\n");
    #endif

    #ifdef JSON
    fprintf(pFile, "\t\t\"communication\":\n");
    fprintf(pFile, "\t\t\{\n");
    fprintf(pFile, "\t\t\t\"volume\":\n");
    fprintf(pFile, "\t\t\t[\n");
    int sum;
    Communication aux;
    for (int j=0; j<all_info.PEs_communication_inside.size(); j++){
        //fprintf(pFile, "\t\t\t\t\"router\":{\n");
        aux = all_info.PEs_communication_inside[j];
        sum = aux.cont_msgs_top + aux.cont_msgs_bottom + aux.cont_msgs_right + aux.cont_msgs_left + aux.cont_msgs_tantofaz;
        fprintf(pFile, "\t\t\t\t\{\n");
        fprintf(pFile, "\t\t\t\t\t\"id\":\"%d\",\n", all_info.PEs_communication_inside[j].id);
        fprintf(pFile, "\t\t\t\t\t\"message_count\":\"%d\",\n", sum);
        fprintf(pFile, "\t\t\t\t\t\"ports\":{\n");

        //sum = (!sum) ? 1;
        
        if(!sum)
            sum = 1;
        fprintf(pFile, "\t\t\t\t\t\t\"top\":\"%.2f%%\",\n", aux.cont_msgs_top*100.0/sum);
        fprintf(pFile, "\t\t\t\t\t\t\"bottom\":\"%.2f%%\",\n", aux.cont_msgs_bottom*100.0/sum);
        fprintf(pFile, "\t\t\t\t\t\t\"left\":\"%.2f%%\",\n", aux.cont_msgs_left * 100.0/sum);
        fprintf(pFile, "\t\t\t\t\t\t\"right\":\"%.2f%%\"\n\t\t\t\t}\n", aux.cont_msgs_right * 100.0/sum);
        if(j == all_info.PEs_communication_inside.size() -1){    
            fprintf(pFile, "\t\t\t\t}\n");
        }else
        {
            fprintf(pFile, "\t\t\t\t},\n");
        }
        
    }
    fprintf(pFile, "\t\t\t]\n");
    fprintf(pFile, "\t\t},\n");
    //fprintf(pFile, "\t\t\t</volume>\n");
    //fprintf(pFile, "\t\t</communication>\n");
    #endif


    // --------------------------
    // ---- end communication ---
    // --------------------------


    // --------------------------
    // -------- Execution -------
    // --------------------------
    #ifdef XML
    fprintf(pFile, "\t\t<execution>\n");
    for (int j=0; j<all_info.Processors_execution_inside.size(); j++){
        fprintf(pFile, "\t\t\t<proc>\n");
        fprintf(pFile, "\t\t\t\t<id>Proc%d</id>\n", all_info.Processors_execution_inside[j].id);
        fprintf(pFile, "\t\t\t\t<request>\"%d\"</request>\n", all_info.Processors_execution_inside[j].cont_msgs_proc);        
        fprintf(pFile, "\t\t\t</proc>\n");
        // if(j == all_info.Processors_execution_inside.size() - 1){
        //     fprintf(pFile, "\t\t\t}\n");
        // }else{
        //     fprintf(pFile, "\t\t\t},\n");
        // }   
    }
    fprintf(pFile, "\t\t</execution>\n");
    #endif



    #ifdef JSON
    fprintf(pFile, "\t\t\"execution\":\n");
    fprintf(pFile, "\t\t[\n");
    for (int j=0; j<all_info.Processors_execution_inside.size(); j++){
        fprintf(pFile, "\t\t\t{\n");
        fprintf(pFile, "\t\t\t\t\"id\":\"Proc%d\",\n", all_info.Processors_execution_inside[j].id);
        fprintf(pFile, "\t\t\t\t\"request\":\"%d\"\n", all_info.Processors_execution_inside[j].cont_msgs_proc);        


        if(j == all_info.Processors_execution_inside.size() - 1){
            fprintf(pFile, "\t\t\t}\n");
        }else{
            fprintf(pFile, "\t\t\t},\n");
        }
        
    }
    fprintf(pFile, "\t\t]\n");
    //fprintf(pFile, "\t\t</execution>\n");
    #endif


    // --------------------------
    // ------ End Execution -----
    // --------------------------

    // END Project
    #ifdef JSON
    printf("\t}\n");
    #endif
    #ifdef XML
    printf("</project>\n");
    fprintf(pFile,"</project>\n");
    #endif



    // End JSON
    #ifdef JSON
    printf("}\n");
    fprintf(pFile,"}\n");
    #endif


   fclose(pFile);
}
