//
// Created by amr on 25/01/2020.
//

#ifndef ACOUSTIC2ND_RTM_TRACE_WRITER_PARSER_H
#define ACOUSTIC2ND_RTM_TRACE_WRITER_PARSER_H

#include <concrete-components/acoustic_second_components.h>
#include <parsers/configuration_parser.h>

TraceWriter *parse_trace_writer_acoustic_iso_openmp_second(ConfigMap map);
TraceWriter *parse_trace_writer_acoustic_iso_openmp_first(ConfigMap map);

#endif // ACOUSTIC2ND_RTM_TRACE_WRITER_PARSER_H
