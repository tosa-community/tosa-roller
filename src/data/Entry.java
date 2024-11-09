package tosaroller.data;

/**
 * The base class for role-list entries
 *
 * @author salade malade
 * @version 1.0.0
 */
abstract class Entry
{
  private String _name;
  private String[] _aliases;

  /**
   * Constructor of list entry class
   *
   * @param name name of the list entry
   * @param aliases array of the possible aliases for the list entry
   */
  public Entry(String name, String[] aliases)
  {
    _name = name;
    _aliases = aliases;
  }
}
