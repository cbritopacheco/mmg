/* =============================================================================
**  This file is part of the mmg software package for the tetrahedral
**  mesh modification.
**  Copyright (c) Bx INP/CNRS/Inria/UBordeaux/UPMC, 2004-
**
**  mmg is free software: you can redistribute it and/or modify it
**  under the terms of the GNU Lesser General Public License as published
**  by the Free Software Foundation, either version 3 of the License, or
**  (at your option) any later version.
**
**  mmg is distributed in the hope that it will be useful, but WITHOUT
**  ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
**  FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
**  License for more details.
**
**  You should have received a copy of the GNU Lesser General Public
**  License and of the GNU General Public License along with mmg (in
**  files COPYING.LESSER and COPYING). If not, see
**  <http://www.gnu.org/licenses/>. Please read their terms carefully and
**  use this copy of the mmg distribution only if you accept them.
** =============================================================================
*/

/**
 * \file common/API_functions.c
 * \brief C API functions definitions for MMG library.
 * \author Charles Dapogny (UPMC)
 * \author Cécile Dobrzynski (Bx INP/Inria/UBordeaux)
 * \author Pascal Frey (UPMC)
 * \author Algiane Froehly (Inria/UBordeaux)
 * \version 5
 * \date 03 2014
 * \copyright GNU Lesser General Public License.
 *
 * \note This file contains some internal functions for the API, see the \a
 * common/libmmgcommon.h, \a mmgs/libmmgs.h and \a mmg3d/libmmg3d.h header files
 * for the documentation of all the usefull user's API functions.
 *
 * C API for MMG library.
 *
 */

#include "mmgcommon.h"

/**
 * \param mesh pointer toward the mesh structure.
 *
 * Initialization of the input parameters.
 *
 */
void MMG5_Init_parameters(MMG5_pMesh mesh) {

  memset(&mesh->info,0, sizeof(MMG5_Info));

  /* default values for integers */
  /* [-1..10], Tune level of imprim */
  mesh->info.imprim   =  1;
  /* [0/1]    ,Turn on/off levelset meshing */
  mesh->info.iso      =  MMG5_OFF;
  mesh->info.isoref   =  MG_ISO;
  /* [n/-1]   ,Set memory size to n Mbytes/keep the default value */
  mesh->info.mem      = MMG5_NONSET_MEM;
  /* [0/1]    ,Turn on/off debug mode */
  mesh->info.ddebug   =  MMG5_OFF;
  /* [n]      ,number of local parameters */
  mesh->info.npar     =  MMG5_OFF;
  /* [0/1]    ,avoid/allow point insertion/deletion */
  mesh->info.noinsert =  MMG5_OFF;
  /* [0/1]    ,avoid/allow edge or face flipping */
  mesh->info.noswap   =  MMG5_OFF;
  /* [0/1]    ,avoid/allow point relocation */
  mesh->info.nomove   =  MMG5_OFF;
  /* [n]    ,number of user-defined references */
  mesh->info.nmat = MMG5_OFF;
  /* [-1/val]    ,Turn off/on the removal of small bubles in levelset meshing */
  mesh->info.rmc      =  MMG5_NONSET;
  /* [0/1]    ,avoid/allow  */
  mesh->info.nosizreq =  MMG5_OFF;


  /* default values for doubles */
  /* angle detection */
  mesh->info.dhd      = MMG5_ANGEDG;
  /* minimal mesh size */
  mesh->info.hmin     = MMG5_NONSET_HMIN;
  /* maximal mesh size */
  mesh->info.hmax     = MMG5_NONSET_HMAX;
  /* constant mesh size */
  mesh->info.hsiz     = MMG5_NONSET_HSIZ;
  /* control Hausdorff */
  mesh->info.hausd    = MMG5_HAUSD;
  /* control gradation */
  mesh->info.hgrad    = MMG5_HGRAD;
  /* control gradation on required entities */
  mesh->info.hgradreq = MMG5_HGRADREQ;

  /* default values for pointers */
  /* list of user-defined references */
  mesh->info.mat = NULL;

  /** MMG3D_IPARAM_lag is used by mmg3d only but need to be negative in the
   * scaleMesh function */
  mesh->info.lag      = MMG5_LAG;

  /* initial value for memMax and gap */
  mesh->gap = MMG5_GAP;
  mesh->memMax = MMG5_memSize();
  if ( mesh->memMax ) {
    /* maximal memory = 50% of total physical memory */
    mesh->memMax = (size_t)(mesh->memMax*MMG5_MEMPERCENT);
  } else {
    /* default value = 800 MB */
    printf("  Maximum memory set to default value: %d MB.\n",MMG5_MEMMAX);
    mesh->memMax = MMG5_MEMMAX << MMG5_BITWIZE_MB_TO_B;
  }

}


/**
 * \param mesh pointer toward the mesh structure.
 * \param sol pointer toward the sol structure.
 *
 * Initialize file names to their default values.
 *
 */
void MMG5_Init_fileNames(MMG5_pMesh mesh,MMG5_pSol sol
  ) {
  MMG5_Set_inputMeshName(mesh,"");
  MMG5_Set_outputMeshName(mesh,"");

  if ( sol ) {
    MMG5_Set_inputSolName(mesh,sol,"");
    MMG5_Set_outputSolName(mesh,sol,"");
  }

  return;
}

/**
 * \param mesh pointer toward the mesh structure.
 * \param meshin input mesh name.
 * \return 1 if success, 0 if fail
 *
 * Set the name of input mesh.
 *
 */
int MMG5_Set_inputMeshName(MMG5_pMesh mesh, const char* meshin) {

  if ( mesh->namein ){
    MMG5_DEL_MEM(mesh,mesh->namein);
  }

  if ( meshin && strlen(meshin) ) {
    MMG5_ADD_MEM(mesh,(strlen(meshin)+1)*sizeof(char),"input mesh name",
                  fprintf(stderr,"  Exit program.\n");
                  return 0);
    MMG5_SAFE_CALLOC(mesh->namein,strlen(meshin)+1,char,return 0);
    strcpy(mesh->namein,meshin);
  }
  else {
    MMG5_ADD_MEM(mesh,10*sizeof(char),"input mesh name",
                  fprintf(stderr,"  Exit program.\n");
                  return 0);
    MMG5_SAFE_CALLOC(mesh->namein,10,char,return 0);
    strcpy(mesh->namein,"mesh.mesh");
    if ( (mesh->info.imprim > 5) || mesh->info.ddebug ) {
      fprintf(stderr,"\n  ## Warning: %s: no name given for input mesh.\n",__func__);
      fprintf(stderr,"              Use of default value \"mesh.mesh\".\n");
    }
  }
  return 1;
}

/**
 * \param mesh pointer toward the mesh structure.
 * \param sol pointer toward the sol structure.
 * \param solin name of the input solution file.
 * \return 1 if success, 0 if fail
 *
 * Set the name of input solution file.
 *
 */
int MMG5_Set_inputSolName(MMG5_pMesh mesh,MMG5_pSol sol, const char* solin) {
  char *ptr;

  if ( sol->namein )
    MMG5_DEL_MEM(mesh,sol->namein);

  if ( solin && strlen(solin) ) {
    MMG5_ADD_MEM(mesh,(strlen(solin)+1)*sizeof(char),"input sol name",
                  fprintf(stderr,"  Exit program.\n");
                  return 0);
    MMG5_SAFE_CALLOC(sol->namein,strlen(solin)+1,char,return 0);
    strcpy(sol->namein,solin);
  }
  else {
    if ( mesh->namein && strlen(mesh->namein) ) {
      int mesh_len = strlen(mesh->namein)+1;
      MMG5_SAFE_CALLOC(sol->namein,mesh_len,char,return 0);
      strcpy(sol->namein,mesh->namein);
      ptr = strstr(sol->namein,".mesh");
      if ( ptr ) {
        /* the sol file is renamed with the meshfile without extension */
        *ptr = '\0';
        MMG5_SAFE_REALLOC(sol->namein,mesh_len,(strlen(sol->namein)+1),char,
                           "input sol name",return 0);
      }
      MMG5_ADD_MEM(mesh,(strlen(sol->namein)+1)*sizeof(char),"input sol name",
                    fprintf(stderr,"  Exit program.\n");
                    return 0);
    }
    else {
      MMG5_ADD_MEM(mesh,9*sizeof(char),"input sol name",
                    fprintf(stderr,"  Exit program.\n");
                    return 0);
      MMG5_SAFE_CALLOC(sol->namein,9,char,return 0);
      strcpy(sol->namein,"mesh.sol");
    }
  }
  return 1;
}

/**
 * \param mesh pointer toward the mesh structure.
 * \param meshout name of the output mesh file.
 * \return 1 if success, 0 if fail.
 *
 * Set the name of output mesh file.
 *
 */
int MMG5_Set_outputMeshName(MMG5_pMesh mesh, const char* meshout) {
  int  fmt = MMG5_FMT_MeditASCII,fmtin;
  char *ptr,*ptrin;

  if ( mesh->nameout )
    MMG5_DEL_MEM(mesh,mesh->nameout);

  if ( meshout && strlen(meshout) ) {
    ptr   = strrchr(meshout, '.');

    MMG5_ADD_MEM(mesh,(strlen(meshout)+7)*sizeof(char),"output mesh name",
                  fprintf(stderr,"  Exit program.\n");
                  return 0);
    MMG5_SAFE_CALLOC(mesh->nameout,strlen(meshout)+7,char,return 0);
    strcpy(mesh->nameout,meshout);

    if ( ( ptr && MMG5_Get_format(ptr,0)==MMG5_FMT_Unknown ) || (!ptr) || ptr == meshout ) {
      /* No extension */
      ptrin   = MMG5_Get_filenameExt(mesh->namein);
      fmtin   = MMG5_Get_format(ptrin,MMG5_FMT_MeditASCII);
      fmt     = MMG5_FMT_Unknown;
    }
    strcpy(mesh->nameout,meshout);

    if ( fmt == MMG5_FMT_Unknown ) {
      /* No extension */
      switch ( fmtin ) {
      case ( MMG5_FMT_GmshASCII ):
        strcat(mesh->nameout,".msh");
        break;
      case ( MMG5_FMT_GmshBinary ):
        strcat(mesh->nameout,".mshb");
        break;
      case ( MMG5_FMT_VtkVtu ):
        strcat(mesh->nameout,".vtu");
        break;
      case ( MMG5_FMT_VtkVtp ):
        strcat(mesh->nameout,".vtp");
        break;
      case ( MMG5_FMT_VtkVtk ):
        strcat(mesh->nameout,".vtk");
        break;
      case ( MMG5_FMT_MeditBinary ):
        strcat(mesh->nameout,".meshb");
        break;
      case ( MMG5_FMT_MeditASCII ): default:
        strcat(mesh->nameout,".mesh");
        break;
      }
    }
  }
  else {
    if ( mesh->namein && strlen(mesh->namein) ) {
      MMG5_ADD_MEM(mesh,(strlen(mesh->namein)+9)*sizeof(char),"output mesh name",
                    fprintf(stderr,"  Exit program.\n");
                    return 0);
      MMG5_SAFE_CALLOC(mesh->nameout,strlen(mesh->namein)+9,char,return 0);
      strcpy(mesh->nameout,mesh->namein);

      ptr   = MMG5_Get_filenameExt(mesh->nameout);
      fmt   = MMG5_Get_format(ptr,MMG5_FMT_MeditASCII);

      if ( ptr ) *ptr = '\0';

      switch ( fmt ) {

      case ( MMG5_FMT_GmshASCII ):
        strcat(mesh->nameout,".o.msh");
        break;
      case ( MMG5_FMT_GmshBinary ):
        strcat(mesh->nameout,".o.mshb");
        break;
      case ( MMG5_FMT_VtkVtu ):
        strcat(mesh->nameout,".o.vtu");
        break;
      case ( MMG5_FMT_VtkVtp ):
        strcat(mesh->nameout,".o.vtp");
        break;
      case ( MMG5_FMT_VtkVtk ):
        strcat(mesh->nameout,".o.vtk");
        break;
      case ( MMG5_FMT_MeditBinary ):
        strcat(mesh->nameout,".o.meshb");
        break;
      case ( MMG5_FMT_MeditASCII ): default:
        strcat(mesh->nameout,".o.mesh");
        break;
      }
    }
    else {
      MMG5_ADD_MEM(mesh,12*sizeof(char),"output mesh name",
                    fprintf(stderr,"  Exit program.\n");
                    return 0);
      MMG5_SAFE_CALLOC(mesh->nameout,12,char,return 0);
      if ( (mesh->info.imprim > 5) || mesh->info.ddebug ) {
        fprintf(stderr,"\n  ## Warning: %s: no name given for output mesh.\n",
                __func__);
        fprintf(stderr,"              Use of default value \"mesh.o.mesh\".\n");
      }
      strcpy(mesh->nameout,"mesh.o.mesh");
    }
  }
  return 1;
}


/**
 * \param mesh pointer toward the mesh structure.
 * \param sol pointer toward the sol structure.
 * \param solout name of the output solution file.
 * \return 0 if failed, 1 otherwise.
 *
 *  Set the name of output solution file.
 *
 */
int MMG5_Set_outputSolName(MMG5_pMesh mesh,MMG5_pSol sol, const char* solout) {
  char *ptr;
  int oldsize;

  if ( sol->nameout )
    MMG5_DEL_MEM(mesh,sol->nameout);

  if ( solout && strlen(solout) ) {
    MMG5_ADD_MEM(mesh,(strlen(solout)+1)*sizeof(char),"output sol name",
                  fprintf(stderr,"  Exit program.\n");
                  return 0);
    MMG5_SAFE_CALLOC(sol->nameout,strlen(solout)+1,char,return 0);
    strcpy(sol->nameout,solout);
  }
  else {
    if ( mesh->nameout && strlen(mesh->nameout) ) {
      ptr = strstr(mesh->nameout,".mesh");
      if ( ptr ) {
        MMG5_SAFE_CALLOC(sol->nameout,strlen(mesh->nameout)+1,char,return 0);
        oldsize = strlen(mesh->nameout)+1;
      }
      else {
        MMG5_SAFE_CALLOC(sol->nameout,strlen(mesh->nameout)+6,char,return 0);
        oldsize = strlen(mesh->nameout)+6;
      }
      strcpy(sol->nameout,mesh->nameout);
      ptr = strstr(sol->nameout,".mesh");
      if ( ptr )
        /* the sol file is renamed with the meshfile without extension */
        *ptr = '\0';
      strcat(sol->nameout,".sol");
      MMG5_ADD_MEM(mesh,(strlen(sol->nameout)+1)*sizeof(char),"output sol name",
                    fprintf(stderr,"  Exit program.\n");
                    return 0);
      MMG5_SAFE_REALLOC(sol->nameout,oldsize,(strlen(sol->nameout)+1),char,
                         "output sol name",return 0);
    }
    else {
      fprintf(stderr,"\n  ## Error: %s: no name for output mesh. please, use",
              __func__);
      fprintf(stderr," the MMG5_Set_outputMeshName to set the mesh name.\n");
      return 0;
    }
  }
  return 1;
}

void MMG5_Set_constantSize(MMG5_pMesh mesh,MMG5_pSol met,double hsiz) {
  MMG5_pPoint ppt;
  int         k,iadr;

  if ( met->size == 1 ) {
    for (k=1; k<=mesh->np; k++) {
      ppt = &mesh->point[k];
      if ( !MG_VOK(ppt) ) continue;
      met->m[k] = hsiz;
    }
  }
  else {
    hsiz    = 1./(hsiz*hsiz);

    if ( mesh->dim==2 ) {
      for (k=1; k<=mesh->np; k++) {
        ppt = &mesh->point[k];
        if ( !MG_VOK(ppt) ) continue;

        iadr           = 3*k;
        met->m[iadr]   = hsiz;
        met->m[iadr+1] = 0.;
        met->m[iadr+2] = hsiz;
      }
    }
    else {
      assert ( mesh->dim==3 );
      for (k=1; k<=mesh->np; k++) {
        ppt = &mesh->point[k];
        if ( !MG_VOK(ppt) ) continue;

        iadr           = 6*k;
        met->m[iadr]   = hsiz;
        met->m[iadr+1] = 0.;
        met->m[iadr+2] = 0.;
        met->m[iadr+3] = hsiz;
        met->m[iadr+4] = 0.;
        met->m[iadr+5] = hsiz;
      }
    }
  }

  return;
}

int MMG5_Free_allSols(MMG5_pMesh mesh,MMG5_pSol *sol) {
  int i;

  if ( sol ) {
    if ( mesh->nsols ) {
      for ( i=0; i<mesh->nsols; ++i ) {
        MMG5_DEL_MEM(mesh,(*sol)[i].m);
      }
    }
    MMG5_DEL_MEM(mesh,(*sol));
  }

  return 1;
}

/**
 * \param mesh pointer toward the mesh structure.
 * \param sol pointer toward the sol structure.
 *
 * Structures unallocation before return (common structures between all codes).
 *
 */
void MMG5_Free_structures(MMG5_pMesh mesh,MMG5_pSol sol){

  if ( mesh->point )
    MMG5_DEL_MEM(mesh,mesh->point);

  if ( mesh->xpoint )
    MMG5_DEL_MEM(mesh,mesh->xpoint);

  if ( mesh->edge )
    MMG5_DEL_MEM(mesh,mesh->edge);

  if ( mesh->adja )
    MMG5_DEL_MEM(mesh,mesh->adja);

  if ( mesh->tria )
    MMG5_DEL_MEM(mesh,mesh->tria);

  if ( mesh->adjt )
    MMG5_DEL_MEM(mesh,mesh->adjt);

  /* sol */
  if ( sol && sol->m )
    MMG5_DEL_MEM(mesh,sol->m);

  /* mesh->info */
  if ( mesh->info.npar && mesh->info.par )
    MMG5_DEL_MEM(mesh,mesh->info.par);

  if ( mesh->info.nmat ) {
    if( mesh->info.mat )
      MMG5_DEL_MEM(mesh,mesh->info.mat);
    if( mesh->info.invmat.lookup )
      MMG5_DEL_MEM(mesh,mesh->info.invmat.lookup);
  }

  if ( mesh->info.imprim>5 || mesh->info.ddebug ) {
    printf("  MEMORY USED AT END (Bytes) %zu\n",mesh->memCur);
  }

  return;
}

/**
 * \param mesh pointer toward the mesh structure.
 * \param met pointer toward the sol structure.
 *
 * File name deallocations before return.
 *
 */
void MMG5_mmgFree_names(MMG5_pMesh mesh,MMG5_pSol met){

  /* mesh */
  if ( mesh->nameout ) {
    MMG5_DEL_MEM(mesh,mesh->nameout);
  }

  if ( mesh->namein ) {
    MMG5_DEL_MEM(mesh,mesh->namein);
  }

  /* met */
  if ( met ) {
    if ( met->namein ) {
      MMG5_DEL_MEM(mesh,met->namein);
    }

    if ( met->nameout ) {
      MMG5_DEL_MEM(mesh,met->nameout);
    }
  }
}

inline
int MMG5_Set_defaultTruncatureSizes(MMG5_pMesh mesh,int8_t sethmin,int8_t sethmax) {

  if ( !sethmin ) {
    if ( sethmax ) {
      mesh->info.hmin  = MG_MIN ( MMG5_HMINCOE, MMG5_HMINCOE * mesh->info.hmax);
    } else {
      mesh->info.hmin  = MMG5_HMINCOE;
    }
  }

  if ( !sethmax ) {
    if ( sethmin ) {
      mesh->info.hmax = MG_MAX ( MMG5_HMAXCOE, 1./MMG5_HMINCOE * mesh->info.hmin);
    }
    else {
      mesh->info.hmax  = MMG5_HMAXCOE;
    }
  }

  if ( mesh->info.hmax < mesh->info.hmin ) {
    assert ( sethmin && sethmax );
    fprintf(stderr,"\n  ## Error: %s: Mismatched options:"
            " minimal mesh size larger than maximal one.\n",__func__);
    return 0;
  }

  return 1;
}

int MMG5_Compute_constantSize(MMG5_pMesh mesh,MMG5_pSol met,double *hsiz) {
  int8_t         sethmin,sethmax;


  if ( mesh->info.hmin > mesh->info.hsiz ) {
    fprintf(stderr,"\n  ## Error: %s: Mismatched options: hmin (%e) is greater"
            " than hsiz (%e). Exit Program.\n",__func__,
            mesh->info.hmin,mesh->info.hsiz);
    return 0;
  }

  if ( mesh->info.hmax > 0. && mesh->info.hmax < mesh->info.hsiz ) {
    fprintf(stderr,"\n  ## Error: %s: Mismatched options: hmax (%e) is lower"
            " than hsiz (%e). Exit Program.\n",__func__,
            mesh->info.hmax,mesh->info.hsiz);
    return 0;
  }

  *hsiz = mesh->info.hsiz;

  sethmin = sethmax = 0;
  if ( mesh->info.hmin > 0. ) {
    sethmin = 1;
    *hsiz    =  MG_MAX(mesh->info.hmin,*hsiz);
  }

  if ( mesh->info.hmax > 0. ) {
    sethmax = 1;
    *hsiz    = MG_MIN(mesh->info.hmax,*hsiz);
  }

  /* Set hmin */
  if ( !sethmin ) {
    if ( sethmax ) {
      mesh->info.hmin  = MG_MIN(0.1*(*hsiz),0.1*mesh->info.hmax);
    } else {
      mesh->info.hmin  = 0.1*(*hsiz);
    }
  }

  /* Set hmax */
  if ( !sethmax ) {
    if ( sethmin ) {
      mesh->info.hmax  = MG_MAX(10.*(*hsiz),10.*mesh->info.hmin);
    } else {
      mesh->info.hmax  = 10.*(*hsiz);
    }
  }

  return 1;
}


const char* MMG5_Get_entitiesName(enum MMG5_entities ent)
{
  switch (ent)
  {
  case MMG5_Noentity:
    return "MMG5_Noentity";
    break;
  case MMG5_Vertex:
    return "MMG5_Vertex";
    break;
  case MMG5_Edg:
    return "MMG5_Edg";
    break;
  case MMG5_Triangle:
    return "MMG5_Triangle";
    break;
  case MMG5_Tetrahedron:
    return "MMG5_Tetrahedron";
    break;
  default:
    return"MMG5_Unknown";
  }
}
const char* MMG5_Get_typeName(enum MMG5_type typ)
{
  switch (typ)
  {
  case MMG5_Notype:
    return "MMG5_Notype";
    break;
  case MMG5_Scalar:
    return "MMG5_Scalar";
    break;
  case MMG5_Vector:
    return "MMG5_Vector";
    break;
  case MMG5_Tensor:
    return "MMG5_Tensor";
    break;
  default:
    return "MMG5_Unknown";
  }
}

int MMG5_Set_multiMat(MMG5_pMesh mesh,MMG5_pSol sol,int ref,
                      int split,int rin,int rex){
  MMG5_pMat mat;
  int k;

  if ( !mesh->info.nmat ) {
    fprintf(stderr,"\n  ## Error: %s: You must set the number of material",__func__);
    fprintf(stderr," with the MMG2D_Set_iparameters function before setting");
    fprintf(stderr," values in multi material structure. \n");
    return 0;
  }
  if ( mesh->info.nmati >= mesh->info.nmat ) {
    fprintf(stderr,"\n  ## Error: %s: unable to set a new material.\n",
            __func__);
    fprintf(stderr,"    max number of materials: %d\n",mesh->info.nmat);
    return 0;
  }
  if ( ref < 0 ) {
    fprintf(stderr,"\n  ## Error: %s: negative references are not allowed.\n",
            __func__);
    return 0;
  }

  for (k=0; k<mesh->info.nmati; k++) {
    mat = &mesh->info.mat[k];

    if ( mat->ref == ref ) {
      mat->dospl = split;
      if ( split ) {
        mat->rin   = rin;
        mat->rex   = rex;
      }
      else {
        mat->rin = mat->ref;
        mat->rex = mat->ref;
      }
      if ( (mesh->info.imprim > 5) || mesh->info.ddebug ) {
        fprintf(stderr,"\n  ## Warning: %s: new materials (interior, exterior)",
                __func__);
        fprintf(stderr," for material of ref %d\n",ref);
      }
      return 1;
    }
  }

  if ( ( split != MMG5_MMAT_Split ) && ( split != MMG5_MMAT_NoSplit ) ) {
    fprintf(stderr,"\n ## Error: %s: unexpected value for the 'split' argument."
            " You must use the MMG5_MMAT_Split or MMG5_MMAT_NpSplit keywords \n",
            __func__);
    return 0;
  }

  mesh->info.mat[mesh->info.nmati].ref   = ref;
  mesh->info.mat[mesh->info.nmati].dospl = split;
  mesh->info.mat[mesh->info.nmati].rin   = rin;
  mesh->info.mat[mesh->info.nmati].rex   = rex;

  mesh->info.nmati++;

  /* Invert the table if all materials have been set */
  if( mesh->info.nmati == mesh->info.nmat )
    if( !MMG5_MultiMat_init(mesh) ) {
      fprintf(stderr,"\n ## Error: %s: unable to create lookup table for multiple materials.\n",
              __func__);
      return 0;
    }

  return 1;
}
