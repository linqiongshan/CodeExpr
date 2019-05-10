//定义一组 option 的关系：互斥，关联，强依赖
//  互斥：该组 option 最多只能使用其中一个
//  关联：option 之前有关联，但没有其它约束性要去，不要求同时使用，不要求互斥。主要用于输出帮助信息时使用
//  强依赖：该组包含的所有 option，要么都不设置，要么都设置

//同一批选项，可能有多种组合场景，不同场景下，对  Option 的依赖关系等要求可能不同，如果体现？
//  e.g. 
//  app -f 1 -a <arga> [-b <argb>]
//  app -f 2 [-a <arga>] -b <argb>


class COptionConstraint {

};

class COption