//
// Created by amr on 18/11/2019.
//

#ifndef ACOUSTIC2ND_RTM_EXTENSION_H
#define ACOUSTIC2ND_RTM_EXTENSION_H

#include <CL/sycl.hpp>
#include <concrete-components/data_units/acoustic_second_grid.h>
#include <skeleton/base/datatypes.h>

/*!
 * Class used for the extensions of velocities for full or window model.
 * Provides standard procedures such as velocity backup and restoring,
 * and logic controlling the extensions however leaves the actual extensions
 * function to subclasses to allow different implementations.
 */
class Extension {
private:
  // used to store the values of velocities before changing them to zeros
  float *backup_array;
  // used to point to the start and end point of the last used window
  Point3D start_point;
  Point3D end_point;
  // The property to be extended by an extensions object.
  float *property_array;
  // The boundary length.
  uint boundary_length;
  // The half length of the used kernel.
  uint half_length;
  // The original_array extensions actual function, will be called in the extend
  // original_array and ReExtend original_array functions.
  // This function will be overridden by each subclass of extensions providing
  // different ways of extending the original_array like extending it by zeros,
  // randomly or otherwise.
  virtual void velocity_extension_helper(float *original_array, int start_x,
                                         int start_z, int start_y, int end_x,
                                         int end_y, int end_z, int nx, int nz,
                                         int ny, uint boundary_length) = 0;
  // Save the original_array values at the boundaries to backup_array.
  void SaveBoundary(float *original_array, int nx, int nz, int ny);
  // Restore the original_array values from backup_array at the boundaries
  // of the last used window to its original location.
  void RestoreBoundary(float *original_array, int nx, int nz, int ny);
  // A helper function responsible of the extensions of the velocities for only
  // the top layer. Called in the ReExtend original_array function. This
  // function will be overridden by each subclass of extensions providing
  // different ways of extending the original_array like extending it by zeros,
  // randomly or otherwise.
  virtual void top_layer_extension_helper(float *original_array, int start_x,
                                          int start_z, int start_y, int end_x,
                                          int end_y, int end_z, int nx, int nz,
                                          int ny, uint boundary_length) = 0;
  // A helper function responsible of the removal of the velocities for only the
  // top layer. Called in the Adjust original_array for backward function. This
  // function will be overridden by each subclass of extensions providing
  // different ways of the removal of the top layer boundary.
  virtual void top_layer_remover_helper(float *original_array, int start_x,
                                        int start_z, int start_y, int end_x,
                                        int end_y, int end_z, int nx, int nz,
                                        int ny, uint boundary_length) = 0;

protected:
  // The grid to extend its velocities/properties.
  GridBox *grid;

public:
  // Constructor.
  Extension();
  // De-constructor.
  virtual ~Extension();
  // Extend the velocities at boundaries.
  void ExtendProperty();
  // In case of window mode extend the velocities at boundaries, re-extend the
  // top layer velocities.
  void ReExtendProperty();
  // Zeros out the top layer in preparation of the backward propagation.
  void AdjustPropertyForBackward();
  // Sets the half length padding used in the extensions operations.
  void SetHalfLength(uint half_length);
  // Sets the boundary length used in the extensions operations.
  void SetBoundaryLength(uint bound_length);
  // Sets the gridbox that the operations are ran on.
  void SetGridBox(GridBox *grid);
  // Sets the property that will be extended by this extensions object.
  void SetProperty(float *property);
};

#endif // ACOUSTIC2ND_RTM_EXTENSION_H
