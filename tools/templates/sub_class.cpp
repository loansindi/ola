[% INCLUDE $licence %]
/*
 * This code has been autogenerated, do not edit
 */

#include <stdlib.h>
#include <string.h>

#include "[% module %][% name %]Msg.h"
#include "[% module %][% name %]Msg[% msg.cls_name%].h"

[% module %][% name %]Msg[% msg.cls_name %]::[% module %][% name %]Msg[% msg.cls_name %]() {}
[% module %][% name %]Msg[% msg.cls_name %]::~[% module %][% name %]Msg[% msg.cls_name %]() {}

[% MACRO method(type) BLOCK %]
/*
 *
 */
const [% type %] *[% module %][% name %]Msg[% msg.cls_name %]::get_[% array.name %](int *len) const {
  *len = [% module %][% name %]Msg[% msg.cls_name %]::[% array.name FILTER upper %]_SIZE;
  return m_[% array.name %];
}

/*
 *
 */
int [% module %][% name %]Msg[% msg.cls_name %]::get_[% array.name %]([% type %] *buf, int len) const {
  int l = [% module %][% name %]Msg[% msg.cls_name %]::[% array.name FILTER upper %]_SIZE < len ? [% module %][% name %]Msg[% msg.cls_name %]::[% array.name FILTER upper %]_SIZE  : len ;
  memcpy(buf, m_[% array.name %], l);
  return l ;
}

/*
 *
 */
void [% module %][% name %]Msg[% msg.cls_name %]::set_[% array.name %]([% type %] *v, int len) {
  int l = sizeof(m_[% array.name %]) < len ? sizeof(m_[% array.name %]) : len;
  memcpy(m_[% array.name %], v, l);
}
[% END %]

[% FOREACH array = msg.array.uint32_t -%]
  [% method('uint32_t') %]
[% END -%]
[% FOREACH array = msg.array.uint16_t -%]
  [% method('uint16_t') %]
[% END -%]
[% FOREACH array = msg.array.uint8_t -%]
  [% method('uint8_t') %]
[% END -%]

/*
 * Pack this object into it's struct
 *
 */
int [% module %][% name %]Msg[% msg.cls_name %]::pack(uint8_t *buf, int len) const {
  int l = sizeof([% lib FILTER lower %]_[% module FILTER lower %]_[% name FILTER lower %]_msg_[% msg.name FILTER lower %]) + [% lib FILTER lower %]_[% module FILTER lower %]_[% name FILTER lower %]_header_size;
  [% lib FILTER lower %]_[% module FILTER lower %]_[% name FILTER lower %]_msg *p = ([% lib FILTER lower %]_[% module FILTER lower %]_[% name FILTER lower %]_msg*) buf;

  if (buf == NULL || len < l) {
    return -1;
  }

  p->op = [% lib FILTER upper %]_[% module FILTER upper %]_[% name FILTER upper %]_MSG_[% msg.name FILTER upper %];

[% FOREACH stat = statics -%]
  p->[% stat.name %] = [% stat.value %];
[% END -%]

[% FOREACH var = msg.uint32_t -%]
  p->data.[% msg.name FILTER lower %].[% var FILTER lower %] = m_[% var %];
[% END -%]
[% FOREACH var = msg.uint16_t -%]
  p->data.[% msg.name FILTER lower %].[% var FILTER lower %] = m_[% var %];
[% END -%]
[% FOREACH var = msg.uint8_t -%]
  p->data.[% msg.name FILTER lower %].[% var FILTER lower %] = m_[% var %];
[% END -%]
[% FOREACH array = msg.array.uint32_t -%]
  memcpy(p->data.[% msg.name FILTER lower %].[% array.name%], m_[% array.name %], [% array.len %]);
[% END -%]
[% FOREACH array = msg.array.uint16_t -%]
  memcpy(p->data.[% msg.name FILTER lower %].[% array.name %], m_[% array.name %], [% array.len %]);
[% END -%]
[% FOREACH array = msg.array.uint8_t -%]
  memcpy(p->data.[% msg.name FILTER lower %].[% array.name %], m_[% array.name %], [% array.len %]);
[% END -%]
  return l;
}


/*
 * Prints the ACSII representation of this message
 */
void [% module %][% name %]Msg[% msg.cls_name %]::print() const {

  printf("[% module %][% name %]Msg[% msg.cls_name %]:\n");
  printf(" op: %hx\n", [% lib FILTER upper %]_[% module FILTER upper %]_[% name FILTER upper %]_MSG_[% msg.name FILTER upper %]);
[% FOREACH var = msg.uint32_t -%]
  printf(" %s %i\n", "[% var FILTER lower %]" , m_[% var %]);
[% END -%]
[% FOREACH var = msg.uint16_t -%]
  printf(" %s %i\n", "[% var FILTER lower %]" , m_[% var %]);
[% END -%]
[% FOREACH var = msg.uint8_t -%]
  printf(" %s %i\n", "[% var FILTER lower %]" , m_[% var %]);
[% END -%]

}
