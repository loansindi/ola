[% INCLUDE $licence %]
/*
 * This code has been autogenerated, do not edit
 */

#ifndef [% module %][% name %]Parser_H
#define [% module %][% name %]Parser_H

#include <stdint.h>
#include "[% module FILTER lower %]_[% name FILTER lower %]_messages.h"

class [% module %][% name %]Parser {

  public:
    [% module %][% name %]Parser();
    ~[% module %][% name %]Parser();
    class [% module %][% name %]Msg *parse(const uint8_t *data, int len);

  private:
    [% module %][% name %]Parser(const [% module %][% name %]Parser&);
    [% module %][% name %]Parser& operator=(const [% module %][% name %]Parser&);

[% FOREACH msg = msgs -%]
    class [% module %][% name %]Msg *parse_[% msg.name %]([% lib FILTER lower %]_[% module FILTER lower %]_[% name FILTER lower %]_msg *req, int len);
[% END -%]
};

#endif
