//
// Created by amr on 18/11/2019.
//

#include "zero_extension.h"
#include <concrete-components/data_units/acoustic_dpc_computation_parameters.h>

using namespace cl::sycl;

void ZeroExtension::velocity_extension_helper(float *property_array,
                                              int start_x, int start_z,
                                              int start_y, int end_x, int end_y,
                                              int end_z, int nx, int nz, int ny,
                                              uint boundary_length) {
  /*!
   * change the values of velocities at boundaries (HALF_LENGTH excluded) to
   * zeros the start for x , y and z is at HALF_LENGTH and the end is at (nx -
   * HALF_LENGTH) or (ny - HALF_LENGTH) or (nz- HALF_LENGTH)
   */
  int nz_nx = nx * nz;

  // In case of 2D
  if (ny == 1) {
    end_y = 1;
    start_y = 0;
  } else {
    // general case for 3D
    /*!putting zero values for velocities at the boundaries for y and with all x
     * and z */
    AcousticDpcComputationParameters::device_queue->submit([&](handler &cgh) {
      auto global_range =
          range<3>(end_x - start_x, boundary_length, end_z - start_z);
      auto local_range = range<3>(1, 1, 1);
      auto global_nd_range = nd_range<3>(global_range, local_range);

      cgh.parallel_for<class Zero_velocity_extension_Y>(
          global_nd_range, [=](nd_item<3> it) {
            int column = it.get_global_id(0) + start_x;
            int depth = it.get_global_id(1);
            int row = it.get_global_id(2) + start_z;

            /*!for values from y = HALF_LENGTH TO y = HALF_LENGTH
             * +BOUND_LENGTH*/
            property_array[(depth + start_y) * nz_nx + row * nx + column] = 0;
            /*!for values from y = ny-HALF_LENGTH TO y =
             * ny-HALF_LENGTH-BOUND_LENGTH*/
            property_array[(end_y - 1 - depth) * nz_nx + row * nx + column] = 0;
          });
    });
  }
  /*!putting zero values for velocities at the boundaries for X and with all Y
   * and Z */
  AcousticDpcComputationParameters::device_queue->submit([&](handler &cgh) {
    auto global_range =
        range<3>(boundary_length, end_y - start_y, end_z - start_z);
    auto local_range = range<3>(1, 1, 1);
    auto global_nd_range = nd_range<3>(global_range, local_range);

    cgh.parallel_for<class Zero_velocity_extension_X>(
        global_nd_range, [=](nd_item<3> it) {
          int column = it.get_global_id(0);
          int depth = it.get_global_id(1) + start_y;
          int row = it.get_global_id(2) + start_z;

          /*!for values from y = HALF_LENGTH TO y = HALF_LENGTH +BOUND_LENGTH*/
          property_array[depth * nz_nx + row * nx + column + start_x] = 0;
          /*!for values from y = ny-HALF_LENGTH TO y =
           * ny-HALF_LENGTH-BOUND_LENGTH*/
          property_array[depth * nz_nx + row * nx + (end_x - 1 - column)] = 0;
        });
  });
  /*!putting zero values for velocities at the boundaries for z and with all x
   * and y */
  AcousticDpcComputationParameters::device_queue->submit([&](handler &cgh) {
    auto global_range =
        range<3>(end_x - start_x, end_y - start_y, boundary_length);
    auto local_range = range<3>(1, 1, 1);
    auto global_nd_range = nd_range<3>(global_range, local_range);

    cgh.parallel_for<class Zero_velocity_extension_Z>(
        global_nd_range, [=](nd_item<3> it) {
          int column = it.get_global_id(0) + start_x;
          int depth = it.get_global_id(1) + start_y;
          int row = it.get_global_id(2);

          /*!for values from y = HALF_LENGTH TO y = HALF_LENGTH +BOUND_LENGTH*/
          property_array[depth * nz_nx + (start_z + row) * nx + column] = 0;
          /*!for values from y = ny-HALF_LENGTH TO y =
           * ny-HALF_LENGTH-BOUND_LENGTH*/
          property_array[depth * nz_nx + (end_z - 1 - row) * nx + column] = 0;
        });
  });
}

void ZeroExtension::top_layer_extension_helper(
    float *property_array, int start_x, int start_z, int start_y, int end_x,
    int end_y, int end_z, int nx, int nz, int ny, uint boundary_length) {
  // Do nothing, no top layer to extend in random boundaries.
}

void ZeroExtension::top_layer_remover_helper(float *property_array, int start_x,
                                             int start_z, int start_y,
                                             int end_x, int end_y, int end_z,
                                             int nx, int nz, int ny,
                                             uint boundary_length) {
  // Do nothing, no top layer to remove in random boundaries.
}
